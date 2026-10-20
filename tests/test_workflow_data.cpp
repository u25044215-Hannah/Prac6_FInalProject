#include "doctest.h"
#include "TividyException.h"
#include "WorkflowData.h"

TEST_CASE("WorkflowData stores and returns values") {
    WorkflowData d;
    CHECK(d.empty());
    d.set("priority", "STAT");
    CHECK(d.has("priority"));
    CHECK(d.get("priority") == "STAT");
    CHECK(d.size() == 1);
}

TEST_CASE("WorkflowData overwrites an existing key") {
    WorkflowData d;
    d.set("k", "1");
    d.set("k", "2");
    CHECK(d.get("k") == "2");
    CHECK(d.size() == 1);
}

TEST_CASE("WorkflowData reports a missing key as NotFoundException") {
    WorkflowData d;
    CHECK_FALSE(d.has("nope"));
    CHECK_THROWS_AS(d.get("nope"), NotFoundException);
    CHECK(d.getOr("nope", "fallback") == "fallback");
}

TEST_CASE("WorkflowData remove and keys") {
    WorkflowData d;
    d.set("b", "2");
    d.set("a", "1");
    std::vector<std::string> keys = d.keys();
    REQUIRE(keys.size() == 2);
    CHECK(keys[0] == "a");   // sorted
    CHECK(keys[1] == "b");
    CHECK(d.remove("a"));
    CHECK_FALSE(d.remove("a"));
    CHECK_FALSE(d.has("a"));
}

TEST_CASE("Copying WorkflowData gives an independent copy") {
    WorkflowData original;
    original.set("amount", "100");
    WorkflowData copy = original;
    copy.set("amount", "999");
    CHECK(original.get("amount") == "100");
    CHECK(copy.get("amount") == "999");
    CHECK(original != copy);
}
