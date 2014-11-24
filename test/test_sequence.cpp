#include "catch.hpp"
#include "utils.h"

TEST_CASE( "Behavior Tree Sequence", "[bt_seq]" ) {
    VirtualMachine vm;
    MockSequence sequence;
    SECTION( "single" ) {
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 0);
    }

    MockAction action_foo;
    sequence.inner_node.children.push_back(action_foo.inner_node);
    SECTION( "[S]" ) {
        action_foo.update_result = BH_SUCCESS;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_SUCCESS);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_SUCCESS);
        REQUIRE(sequence.counter.prepare == 2);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    SECTION( "[F]" ) {
        action_foo.update_result = BH_FAILURE;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 2);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    SECTION( "[R]" ) {
        action_foo.update_result = BH_RUNNING;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    MockAction action_bar;
    sequence.inner_node.children.push_back(action_bar.inner_node);
    SECTION( "[S,S]" ) {
        action_foo.update_result = BH_SUCCESS;
        action_bar.update_result = BH_SUCCESS;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_SUCCESS);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_SUCCESS);
        REQUIRE(sequence.counter.prepare == 2);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 4);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 2);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[S,F]" ) {
        action_foo.update_result = BH_SUCCESS;
        action_bar.update_result = BH_FAILURE;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 2);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 4);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 2);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[F,S]" ) {
        action_foo.update_result = BH_FAILURE;
        action_bar.update_result = BH_SUCCESS;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_FAILURE);
        REQUIRE(sequence.counter.prepare == 2);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[S,R]" ) {
        action_foo.update_result = BH_SUCCESS;
        action_bar.update_result = BH_RUNNING;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 3);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[S,R]->[F,R]" ) {
        action_foo.update_result = BH_SUCCESS;
        action_bar.update_result = BH_RUNNING;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        action_foo.update_result = BH_FAILURE;
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 3);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[R,S]->[S,S]" ) {
        action_foo.update_result = BH_RUNNING;
        action_bar.update_result = BH_SUCCESS;
        to_vm(vm, sequence.inner_node);
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_RUNNING);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 1);
        REQUIRE(sequence.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
        action_foo.update_result = BH_SUCCESS;
        vm.tick(nullptr);
        REQUIRE(sequence.child_update_result == BH_SUCCESS);
        REQUIRE(sequence.counter.prepare == 1);
        REQUIRE(sequence.counter.abort == 0);
        REQUIRE(sequence.counter.self_update == 2);
        REQUIRE(sequence.counter.child_update == 3);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
    }
}