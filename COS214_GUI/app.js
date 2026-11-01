
"use strict";

const $ = selector => document.querySelector(selector);

const $$ = selector =>
    Array.from(document.querySelectorAll(selector));

function element(id) {
    const node = document.getElementById(id);

    if (!node) {
        console.warn("Missing HTML element:", id);
    }

    return node;
}

function setText(id, value) {
    const node = element(id);
    if (node) node.textContent = value;
}

function setHTML(id, value) {
    const node = element(id);
    if (node) node.innerHTML = value;
}

function escapeHTML(value) {
    return String(value ?? "").replace(
        /[&<>"']/g,
        char => ({
            "&": "&amp;",
            "<": "&lt;",
            ">": "&gt;",
            '"': "&quot;",
            "'": "&#39;"
        })[char]
    );
}

function formatState(state) {
    const labels = {
        Created: "Created",
        Available: "Available",
        Assigned: "Assigned",
        InProgress: "In progress",
        OnHold: "On hold",
        Completed: "Completed",
        Cancelled: "Cancelled"
    };

    return labels[state] || state || "Unknown";
}

function stateBadge(state) {
    return `
        <span class="badge ${escapeHTML(state)}">
            ${escapeHTML(formatState(state))}
        </span>
    `;
}
let scenarios = [];
let sid = "";
let currentTab = "dashboard";
let selected = null;
let modalCallback = null;

const pageNames = {
    dashboard: [
        "Overview",
        "Design workflows, execute tasks, and monitor progress."
    ],
    builder: [
        "Workflow builder",
        "Create stages and tasks for any industry."
    ],
    board: [
        "Task board",
        "Execute real, saved workflow transitions."
    ],
    reports: [
        "Reports",
        "Live metrics calculated from saved task states."
    ],
    history: [
        "Audit & recovery",
        "Inspect events and restore checkpoints."
    ],
    engine: [
        "C++ test laboratory",
        "Run the original compiled integration harness."
    ]
};

function scenario() {
    return scenarios.find(s => s.id === sid) || null;
}

function getTasks(currentScenario = scenario()) {
    return (currentScenario?.stages || []).flatMap(
        stage => stage.tasks || []
    );
}

function findTask(taskId) {
    return getTasks().find(task => task.id === taskId);
}

function findStage(stageId) {
    return (scenario()?.stages || []).find(
        stage => stage.id === stageId
    );
}

function taskName(taskId) {
    return findTask(taskId)?.name || taskId;
}

function unmetDependencies(task) {
    return (task.depends_on || []).filter(id => {
        const prerequisite = findTask(id);
        return !prerequisite ||
               prerequisite.status !== "Completed";
    });
}

function toast(message, isError = false) {
    const node = element("toast");

    if (!node) {
        console.log(message);
        return;
    }

    node.textContent = message;
    node.classList.toggle("error", isError);
}

async function api(path, options = {}) {
    const response = await fetch(path, options);

    const contentType =
        response.headers.get("content-type") || "";

    let data;

    if (contentType.includes("application/json")) {
        data = await response.json();
    } else {
        const body = await response.text();

        throw new Error(
            "Server returned an unexpected response: " +
            body.slice(0, 200)
        );
    }

    if (!response.ok) {
        throw new Error(
            data.error || "Request failed"
        );
    }

    return data;
}

async function mutate(payload) {
    try {
        await api("/api/workflow", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(payload)
        });

        toast("Changes saved successfully");

        await loadScenarios();

        return true;

    } catch (error) {
        toast(error.message, true);
        alert(error.message);
        return false;
    }
}

async function loadScenarios() {
    try {
        scenarios = await api("/api/scenarios");

        if (!Array.isArray(scenarios)) {
            throw new Error(
                "Invalid scenario data returned by server"
            );
        }

        if (!scenarios.some(s => s.id === sid)) {
            sid = scenarios[0]?.id || "";
            selected = null;
        }

        const selector = element("scenario");

        if (selector) {
            selector.innerHTML = scenarios.length
                ? scenarios.map(s => `
                    <option value="${escapeHTML(s.id)}">
                        ${escapeHTML(s.name)}
                    </option>
                `).join("")
                : '<option value="">No scenarios available</option>';

            selector.value = sid;
        }

        render();

    } catch (error) {
        console.error("Scenario loading failed:", error);
        toast("Cannot load scenarios: " + error.message, true);
    }
}

function go(tab) {
    if (!pageNames[tab]) return;

    currentTab = tab;

    $$("#nav button").forEach(button => {
        button.classList.toggle(
            "active",
            button.dataset.tab === tab
        );
    });

    $$(".page").forEach(page => {
        page.classList.toggle(
            "active",
            page.id === tab
        );
    });

    setText("where", pageNames[tab][0]);
    setText("title", pageNames[tab][0]);
    setText("subtitle", pageNames[tab][1]);

    render();
}

function render() {
    const current = scenario();

    const deleteButton = element("delete-scenario");

    if (deleteButton) {
        deleteButton.disabled =
            !current || Boolean(current.builtin_key);
    }

    if (!current) {
        setHTML(
            "overview-tree",
            '<div class="empty">Create a scenario to begin.</div>'
        );

        setHTML("stats", "");
        setHTML("report-stats", "");
        setHTML("builder-tree", "");
        setHTML("task-board", "");

        return;
    }

    const allTasks = getTasks(current);

    const completed = allTasks.filter(
        task => task.status === "Completed"
    ).length;

    const active = allTasks.filter(
        task => [
            "Assigned",
            "InProgress",
            "OnHold"
        ].includes(task.status)
    ).length;

    const progress = allTasks.length
        ? Math.round(completed / allTasks.length * 100)
        : 0;

    renderDashboard(
        current,
        allTasks,
        completed,
        active,
        progress
    );

    renderBuilder(current);

    renderBoard(current);

    renderReports(current, allTasks, progress);

    if (currentTab === "history") {
        loadHistory();
    }
}

function renderDashboard(
    current,
    allTasks,
    completed,
    active,
    progress
) {
    const statistics = [
        [
            "TOTAL TASKS",
            allTasks.length,
            `Across ${current.stages.length} stages`
        ],
        [
            "COMPLETED",
            completed,
            "Finished successfully"
        ],
        [
            "ACTIVE",
            active,
            "Assigned, running or paused"
        ],
        [
            "PROGRESS",
            `${progress}%`,
            "Based on saved task states"
        ]
    ];

    const statsHTML = statistics.map(item => `
        <article class="stat">
            <span>${item[0]}</span>
            <strong>${item[1]}</strong>
            <small>${escapeHTML(item[2])}</small>
        </article>
    `).join("");

    setHTML("stats", statsHTML);
    setHTML("report-stats", statsHTML);

    const tree = current.stages.length
        ? current.stages.map(stage => `
            <div class="tree-stage">

                <div class="stage-title">
                    ▣ ${escapeHTML(stage.name)}
                    <span class="muted">
                        · ${stage.tasks.length} tasks
                    </span>
                </div>

                ${stage.tasks.map(task => `
                    <div class="tree-task">
                        <span>
                            ${escapeHTML(task.name)}
                        </span>
                        ${stateBadge(task.status)}
                    </div>
                `).join("") || `
                    <div class="muted">
                        No tasks yet
                    </div>
                `}

            </div>
        `).join("")
        : `
            <div class="empty">
                No stages yet.
                Open Workflow Builder to create one.
            </div>
        `;

    setHTML("overview-tree", tree);
}

function renderBuilder(current) {
    const allTasks = getTasks(current);

    setText(
        "builder-summary",
        `${current.stages.length} stages · ` +
        `${allTasks.length} tasks · ` +
        (current.builtin_key
            ? "Predefined scenario"
            : "Custom scenario")
    );

    const tree = current.stages.map(stage => `
        <div class="tree-stage">

            <div
                class="stage-title tree-click"
                data-inspect-stage="${escapeHTML(stage.id)}"
            >
                ▣ ${escapeHTML(stage.name)}
            </div>

            ${stage.tasks.map(task => `
                <div
                    class="tree-task tree-click"
                    data-inspect-task="${escapeHTML(task.id)}"
                >
                    <span>${escapeHTML(task.name)}</span>
                    ${stateBadge(task.status)}
                </div>
            `).join("")}

            <button
                type="button"
                class="soft"
                data-add-to-stage="${escapeHTML(stage.id)}"
            >
                + Add task
            </button>

        </div>
    `).join("");

    setHTML(
        "builder-tree",
        tree || `
            <div class="empty">
                Add a stage to begin building your scenario.
            </div>
        `
    );

    if (selected?.type === "task") {
        const task = findTask(selected.id);

        if (task) {
            inspectTask(task);
        } else {
            selected = null;
            setHTML(
                "inspector",
                '<div class="empty">Select a task.</div>'
            );
        }

    } else if (selected?.type === "stage") {
        const stage = findStage(selected.id);

        if (stage) {
            inspectStage(stage);
        } else {
            selected = null;
            setHTML(
                "inspector",
                '<div class="empty">Select a stage.</div>'
            );
        }
    }
}

function inspectTask(task) {
    selected = {
        type: "task",
        id: task.id
    };

    const prerequisites = task.depends_on?.length
        ? task.depends_on
            .map(id => escapeHTML(taskName(id)))
            .join(", ")
        : "None";

    const blockedBy = getTasks().filter(
        other =>
            other.id !== task.id &&
            (other.depends_on || []).includes(task.id)
    );

    setHTML("inspector", `
        <div class="eyebrow">TASK DETAILS</div>

        <div class="inspector-title">
            ${escapeHTML(task.name)}
        </div>

        <p>
            ${escapeHTML(
                task.description || "No description"
            )}
        </p>

        <p>
            State: ${stateBadge(task.status)}
        </p>

        <p>
            Assignee:
            <b>${escapeHTML(task.assignee || "Unassigned")}</b>
        </p>

        <p>
            Required role:
            <b>${escapeHTML(task.role || "Not specified")}</b>
        </p>

        <p>
            Prerequisites:
            <b>${prerequisites}</b>
        </p>

        <p>
            Created:
            ${escapeHTML(task.created_at || "Unknown")}
        </p>

        ${blockedBy.length ? `
            <p class="muted">
                This task is required by
                ${blockedBy.length} other task(s).
                Those dependencies must be removed
                before deletion.
            </p>
        ` : ""}

        <div class="task-actions">

            <button
                type="button"
                class="primary"
                data-go="board"
            >
                Manage task
            </button>

            <button
                type="button"
                class="soft"
                data-delete-task="${escapeHTML(task.id)}"
                ${blockedBy.length ? "disabled" : ""}
            >
                Delete task
            </button>

        </div>
    `);
}

function inspectStage(stage) {
    selected = {
        type: "stage",
        id: stage.id
    };

    setHTML("inspector", `
        <div class="eyebrow">STAGE DETAILS</div>

        <div class="inspector-title">
            ${escapeHTML(stage.name)}
        </div>

        <p>${stage.tasks.length} tasks</p>

        <button
            type="button"
            class="primary"
            data-add-to-stage="${escapeHTML(stage.id)}"
        >
            + Add task to stage
        </button>
    `);
}

function taskActions(task) {
    const mapping = {
        Created: [
            ["make_available", "Make available"],
            ["cancel", "Cancel"]
        ],
        Available: [
            ["assign", "Assign"],
            ["cancel", "Cancel"]
        ],
        Assigned: [
            ["start", "Start"],
            ["cancel", "Cancel"]
        ],
        InProgress: [
            ["complete", "Complete"],
            ["hold", "Hold"],
            ["cancel", "Cancel"]
        ],
        OnHold: [
            ["resume", "Resume"],
            ["cancel", "Cancel"]
        ],
        Completed: [],
        Cancelled: []
    };

    return mapping[task.status] || [];
}

function renderBoard(current) {
    const filter = element("filter")?.value || "all";

    let taskCount = 0;

    const board = current.stages.map(stage => {
        const cards = stage.tasks
            .filter(task =>
                filter === "all" ||
                task.status === filter
            )
            .map(task => {
                taskCount++;

                const unmet = unmetDependencies(task);

                const waiting =
                    task.status === "Created" &&
                    unmet.length > 0;

                return `
                    <div class="task-card">

                        <div class="task-top">
                            <div>
                                <h3>
                                    ${escapeHTML(task.name)}
                                </h3>

                                <p>
                                    ${escapeHTML(
                                        task.description ||
                                        "No description"
                                    )}
                                </p>
                            </div>

                            ${stateBadge(task.status)}
                        </div>

                        <div class="muted">
                            Assigned to:
                            ${escapeHTML(
                                task.assignee || "Nobody yet"
                            )}
                        </div>

                        ${task.role ? `
                            <div class="muted">
                                Required role:
                                ${escapeHTML(task.role)}
                            </div>
                        ` : ""}

                        ${task.depends_on?.length ? `
                            <div class="muted">
                                Prerequisites:
                                ${task.depends_on
                                    .map(id =>
                                        escapeHTML(taskName(id))
                                    )
                                    .join(", ")}
                            </div>
                        ` : `
                            <div class="muted">
                                No prerequisites
                            </div>
                        `}

                        ${waiting ? `
                            <div class="muted">
                                🔒 Waiting for:
                                ${unmet.map(id =>
                                    escapeHTML(taskName(id))
                                ).join(", ")}
                            </div>
                        ` : ""}

                        <div class="task-actions">

                            ${taskActions(task).map(
                                ([operation, label]) => `
                                    <button
                                        type="button"
                                        class="${
                                            operation === "complete"
                                                ? "primary"
                                                : "soft"
                                        }"
                                        data-task-op="${operation}"
                                        data-task="${escapeHTML(task.id)}"
                                        ${
                                            operation === "make_available" &&
                                            unmet.length
                                                ? "disabled"
                                                : ""
                                        }
                                    >
                                        ${label}
                                    </button>
                                `
                            ).join("") || `
                                <span class="muted">
                                    No further transitions available
                                </span>
                            `}

                        </div>
                    </div>
                `;
            })
            .join("");

        if (!cards) return "";

        return `
            <div class="stage-heading">
                ${escapeHTML(stage.name)}
            </div>
            ${cards}
        `;
    }).join("");

    setHTML(
        "task-board",
        taskCount
            ? board
            : `
                <div class="empty">
                    No tasks match this filter.
                </div>
            `
    );
}

function renderReports(current, allTasks, progress) {
    setText("completion-pct", `${progress}%`);

    const completionBar = element("completion-bar");

    if (completionBar) {
        completionBar.style.width = `${progress}%`;
    }

    const states = [
        "Created",
        "Available",
        "Assigned",
        "InProgress",
        "OnHold",
        "Completed",
        "Cancelled"
    ];

    setHTML(
        "status-chart",
        states.map(state => {
            const count = allTasks.filter(
                task => task.status === state
            ).length;

            const percent = allTasks.length
                ? count / allTasks.length * 100
                : 0;

            return `
                <div class="statusrow">

                    <label>
                        ${formatState(state)}
                    </label>

                    <div class="bar">
                        <div style="width:${percent}%"></div>
                    </div>

                    <small>${count}</small>

                </div>
            `;
        }).join("")
    );

    const completedTasks = allTasks.filter(
        task =>
            task.started_at &&
            task.completed_at
    );

    if (!completedTasks.length) {
        setHTML(
            "cycle-table",
            `
                <div class="empty">
                    Complete a task to see its measured duration.
                </div>
            `
        );

        return;
    }

    const rows = completedTasks.map(task => {
        const start = new Date(task.started_at);
        const end = new Date(task.completed_at);

        const seconds = Math.max(
            0,
            Math.round((end - start) / 1000)
        );

        return `
            <tr>
                <td>${escapeHTML(task.name)}</td>
                <td>${escapeHTML(task.assignee || "Unassigned")}</td>
                <td>${Number.isFinite(seconds) ? seconds + " s" : "—"}</td>
            </tr>
        `;
    }).join("");

    setHTML("cycle-table", `
        <table class="table">
            <thead>
                <tr>
                    <th>Task</th>
                    <th>Assignee</th>
                    <th>Elapsed time</th>
                </tr>
            </thead>

            <tbody>
                ${rows}
            </tbody>
        </table>
    `);
}

async function loadHistory() {
    const requestedScenario = sid;

    if (!requestedScenario) return;

    try {
        const [events, checkpoints] = await Promise.all([
            api(
                "/api/events?scenario_id=" +
                encodeURIComponent(requestedScenario)
            ),
            api(
                "/api/checkpoints?scenario_id=" +
                encodeURIComponent(requestedScenario)
            )
        ]);

        if (
            currentTab !== "history" ||
            requestedScenario !== sid
        ) {
            return;
        }

        setHTML(
            "events",
            events.map(event => `
                <div class="audit">
                    <strong>
                        ${escapeHTML(event.action)}
                    </strong>

                    <small>
                        ${escapeHTML(event.timestamp)}
                        ·
                        ${escapeHTML(event.actor)}
                    </small>

                    <p>
                        ${escapeHTML(event.detail)}
                    </p>
                </div>
            `).join("") || `
                <div class="empty">
                    No audit history yet.
                </div>
            `
        );

        setHTML(
            "checkpoints",
            checkpoints.map(checkpoint => `
                <div class="checkpoint-item">

                    <div>
                        <b>${escapeHTML(checkpoint.name)}</b>

                        <div class="muted">
                            ${escapeHTML(checkpoint.created_at)}
                        </div>
                    </div>

                    <button
                        type="button"
                        class="soft"
                        data-restore="${escapeHTML(checkpoint.id)}"
                    >
                        Restore
                    </button>

                </div>
            `).join("") || `
                <div class="empty">
                    No checkpoints saved yet.
                </div>
            `
        );

    } catch (error) {
        toast(error.message, true);
    }
}

function field(key, label, extra = {}) {
    return {
        key,
        label,
        ...extra
    };
}

function modal(title, description, fields, callback) {
    const dialog = element("modal");

    if (!dialog) {
        alert("Cannot open the form.");
        return;
    }

    setText("modal-title", title);
    setText("modal-desc", description);

    const fieldsHTML = fields.map(f => {
        const required =
            f.required === false ? "" : "required";

        const label = escapeHTML(f.label);

        if (f.kind === "select") {
            return `
                <label class="field">
                    ${label}

                    <select
                        name="${escapeHTML(f.key)}"
                        ${required}
                    >
                        ${f.options.map(option => `
                            <option
                                value="${escapeHTML(option.value)}"
                            >
                                ${escapeHTML(option.label)}
                            </option>
                        `).join("")}
                    </select>
                </label>
            `;
        }

        if (f.kind === "textarea") {
            return `
                <label class="field">
                    ${label}

                    <textarea
                        name="${escapeHTML(f.key)}"
                        rows="3"
                        maxlength="500"
                        ${required}
                    ></textarea>
                </label>
            `;
        }

        return `
            <label class="field">
                ${label}

                <input
                    name="${escapeHTML(f.key)}"
                    maxlength="${f.max || 160}"
                    placeholder="${escapeHTML(
                        f.placeholder || ""
                    )}"
                    ${required}
                >
            </label>
        `;
    }).join("");

    setHTML("modal-fields", fieldsHTML);

    modalCallback = callback;

    dialog.showModal();
}

function dependencyField() {
    const options = [
        {
            value: "",
            label: "No prerequisite"
        },
        ...getTasks().map(task => ({
            value: task.id,
            label: task.name
        }))
    ];

    return field(
        "depends_on",
        "Prerequisite (optional)",
        {
            kind: "select",
            options,
            required: false
        }
    );
}

function addStage() {
    if (!sid) {
        alert("Create or select a scenario first.");
        return;
    }

    modal(
        "Add stage",
        "Stages group related tasks.",
        [
            field(
                "name",
                "Stage name",
                {
                    placeholder: "e.g. Application review"
                }
            )
        ],
        values => mutate({
            action: "add_stage",
            scenario_id: sid,
            ...values
        })
    );
}

function addTask(stageId) {
    if (!stageId) {
        alert("Select a stage first.");
        return;
    }

    modal(
        "Add task",
        "Create a new workflow task. A prerequisite is optional.",
        [
            field(
                "name",
                "Task name",
                {
                    placeholder: "e.g. Verify inventory"
                }
            ),

            dependencyField(),

            field(
                "role",
                "Required role (optional)",
                {
                    required: false,
                    placeholder: "e.g. Manager"
                }
            ),

            field(
                "description",
                "Description (optional)",
                {
                    kind: "textarea",
                    required: false
                }
            )
        ],

        values => mutate({
            action: "add_task",
            stage_id: stageId,

            name: values.name.trim(),

            // Empty means NO prerequisite.
            depends_on: values.depends_on || "",

            role: values.role || "",
            description: values.description || ""
        })
    );
}

function addTaskFromBoard() {
    const current = scenario();

    if (!current?.stages?.length) {
        alert(
            "Please create a stage in Workflow Builder first."
        );
        return;
    }

    modal(
        "Add task",
        "Select a stage and create a new task.",
        [
            field(
                "stage_id",
                "Stage",
                {
                    kind: "select",
                    options: current.stages.map(stage => ({
                        value: stage.id,
                        label: stage.name
                    }))
                }
            ),

            field(
                "name",
                "Task name",
                {
                    placeholder: "e.g. Process application"
                }
            ),

            dependencyField(),

            field(
                "role",
                "Required role (optional)",
                {
                    required: false
                }
            ),

            field(
                "description",
                "Description (optional)",
                {
                    kind: "textarea",
                    required: false
                }
            )
        ],

        values => mutate({
            action: "add_task",

            stage_id: values.stage_id,
            name: values.name.trim(),

            // IMPORTANT: Empty is valid.
            depends_on: values.depends_on || "",

            role: values.role || "",
            description: values.description || ""
        })
    );
}

async function deleteTask(taskId) {
    const task = findTask(taskId);

    if (!task) {
        alert("Task could not be found.");
        return;
    }

    const dependentTasks = getTasks().filter(
        other =>
            other.id !== task.id &&
            (other.depends_on || []).includes(task.id)
    );

    if (dependentTasks.length) {
        alert(
            "This task cannot be deleted because " +
            "other tasks depend on it:\n\n" +
            dependentTasks.map(t => t.name).join("\n")
        );
        return;
    }

    const confirmed = confirm(
        `Delete task "${task.name}"?\n\n` +
        "This action cannot be undone directly."
    );

    if (!confirmed) return;

    const success = await mutate({
        action: "delete_task",
        task_id: taskId
    });

    if (success) {
        selected = null;

        setHTML(
            "inspector",
            '<div class="empty">Select a task or stage.</div>'
        );

        render();
    }
}

async function taskAction(taskId, operation) {
    if (operation === "assign") {
        modal(
            "Assign task",
            "Enter the participant responsible for this task.",
            [
                field(
                    "assignee",
                    "Assignee",
                    {
                        placeholder: "e.g. Operator A"
                    }
                )
            ],

            values => mutate({
                action: "task_action",
                task_id: taskId,
                operation: "assign",
                assignee: values.assignee.trim()
            })
        );

        return;
    }

    await mutate({
        action: "task_action",
        task_id: taskId,
        operation
    });
}

function createScenario() {
    modal(
        "New scenario",
        "Create a workflow for any industry.",
        [
            field(
                "name",
                "Scenario name",
                {
                    placeholder: "e.g. University admissions"
                }
            ),

            field(
                "industry",
                "Industry",
                {
                    placeholder: "e.g. Education"
                }
            ),

            field(
                "description",
                "Purpose (optional)",
                {
                    kind: "textarea",
                    required: false
                }
            )
        ],

        async values => {
            try {
                const created = await api(
                    "/api/workflow",
                    {
                        method: "POST",
                        headers: {
                            "Content-Type": "application/json"
                        },

                        body: JSON.stringify({
                            action: "create_scenario",
                            name: values.name.trim(),
                            industry: values.industry.trim(),
                            description: values.description || ""
                        })
                    }
                );

                sid = created.id;
                selected = null;

                await loadScenarios();

                go("builder");

                toast("Scenario created successfully");

            } catch (error) {
                toast(error.message, true);
                alert(error.message);
            }
        }
    );
}

async function resetScenario() {
    if (!sid) return;

    if (!confirm(
        "Reset all task statuses and assignments?\n\n" +
        "Stages and tasks will be preserved."
    )) {
        return;
    }

    await mutate({
        action: "reset_scenario",
        scenario_id: sid
    });
}

async function deleteScenario() {
    const current = scenario();

    if (!current) return;

    if (current.builtin_key) {
        alert("Predefined scenarios cannot be deleted.");
        return;
    }

    const confirmed = confirm(
        `Permanently delete "${current.name}"?\n\n` +
        "All its tasks and history will be removed."
    );

    if (!confirmed) return;

    const success = await mutate({
        action: "delete_scenario",
        scenario_id: sid
    });

    if (success) {
        selected = null;
    }
}

function createCheckpoint() {
    if (!sid) return;

    modal(
        "Create checkpoint",
        "Save the current workflow execution state.",
        [
            field(
                "name",
                "Checkpoint name",
                {
                    placeholder: "e.g. Before approvals"
                }
            )
        ],

        values => mutate({
            action: "checkpoint",
            scenario_id: sid,
            name: values.name.trim()
        })
    );
}

async function restoreCheckpoint(checkpointId) {
    if (!confirm(
        "Restore task states from this checkpoint?\n\n" +
        "Current task execution states will be replaced."
    )) {
        return;
    }

    await mutate({
        action: "restore",
        checkpoint_id: checkpointId
    });
}

async function runCpp(action) {
    setText("engine-summary", "Running " + action + "...");
    setText(
        "engine-output",
        "Executing C++ program..."
    );

    try {
        const data = await api("/api/run", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                industry:
                    element("demo-industry")?.value || "healthcare",
                action
            })
        });

        const summary = [
            data.ok ? "Completed" : "Error",
            `${data.passed ?? 0} passing checks`,
            `${data.failed ?? 0} failed checks`
        ];

        if (data.progress != null) {
            summary.push(
                `Progress ${data.progress}%`
            );
        }

        setText(
            "engine-summary",
            summary.join(" · ")
        );

        setText(
            "engine-output",
            data.output || "No output available."
        );

    } catch (error) {
        setText(
            "engine-summary",
            "C++ bridge failed"
        );

        setText(
            "engine-output",
            error.message
        );
    }
}

function attachEvents() {
    // Sidebar navigation
    const navigation = element("nav");

    if (navigation) {
        navigation.addEventListener("click", event => {
            const button = event.target.closest(
                "button[data-tab]"
            );

            if (button) {
                go(button.dataset.tab);
            }
        });
    }

    // Global click delegation
    document.body.addEventListener(
        "click",
        async event => {
            const button = event.target.closest("button");

            if (button) {
                if (button.dataset.go) {
                    go(button.dataset.go);
                    return;
                }

                if (button.dataset.addToStage) {
                    addTask(button.dataset.addToStage);
                    return;
                }

                if (button.dataset.deleteTask) {
                    await deleteTask(button.dataset.deleteTask);
                    return;
                }

                if (button.dataset.taskOp) {
                    await taskAction(
                        button.dataset.task,
                        button.dataset.taskOp
                    );
                    return;
                }

                if (button.dataset.restore) {
                    await restoreCheckpoint(
                        button.dataset.restore
                    );
                    return;
                }

                if (button.dataset.run) {
                    await runCpp(button.dataset.run);
                    return;
                }
            }

            // Select stage/task within the workflow tree
            const node = event.target.closest(
                "[data-inspect-stage],[data-inspect-task]"
            );

            if (!node || node.closest("button")) {
                return;
            }

            if (node.dataset.inspectTask) {
                const task = findTask(
                    node.dataset.inspectTask
                );

                if (task) inspectTask(task);

            } else if (node.dataset.inspectStage) {
                const stage = findStage(
                    node.dataset.inspectStage
                );

                if (stage) inspectStage(stage);
            }
        }
    );

    // Scenario selector
    const scenarioSelector = element("scenario");

    if (scenarioSelector) {
        scenarioSelector.addEventListener(
            "change",
            event => {
                sid = event.target.value;
                selected = null;

                setHTML(
                    "inspector",
                    '<div class="empty">Select a stage or task.</div>'
                );

                render();
            }
        );
    }

    // Task state filter
    const filter = element("filter");

    if (filter) {
        filter.addEventListener(
            "change",
            () => {
                if (scenario()) {
                    renderBoard(scenario());
                }
            }
        );
    }

    // New scenario
    element("new-scenario")?.addEventListener(
        "click",
        createScenario
    );

    // Add stage
    element("add-stage")?.addEventListener(
        "click",
        addStage
    );

    // Add task from board
    element("board-add")?.addEventListener(
        "click",
        addTaskFromBoard
    );

    // Reset scenario
    element("reset-scenario")?.addEventListener(
        "click",
        resetScenario
    );

    // Delete scenario
    element("delete-scenario")?.addEventListener(
        "click",
        deleteScenario
    );

    // Checkpoint
    element("checkpoint")?.addEventListener(
        "click",
        createCheckpoint
    );

    // Modal close buttons
    element("close-modal")?.addEventListener(
        "click",
        () => element("modal")?.close()
    );

    element("cancel-modal")?.addEventListener(
        "click",
        () => element("modal")?.close()
    );

    // Modal submit
    element("modal-form")?.addEventListener(
        "submit",
        event => {
            event.preventDefault();

            const form = event.currentTarget;

            if (!form.reportValidity()) {
                return;
            }

            const values = Object.fromEntries(
                new FormData(form).entries()
            );

            // Explicitly allow empty prerequisites.
            if ("depends_on" in values) {
                values.depends_on =
                    values.depends_on || "";
            }

            const callback = modalCallback;

            element("modal")?.close();

            modalCallback = null;

            if (callback) {
                callback(values);
            }
        }
    );
}

async function initialize() {
    attachEvents();
    await loadScenarios();
}

if (document.readyState === "loading") {
    document.addEventListener(
        "DOMContentLoaded",
        initialize,
        { once: true }
    );
} else {
    initialize();
}
