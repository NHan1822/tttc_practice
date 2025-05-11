// RUN: %clang_cc1 -load %pluginlib -plugin mark-unused -plugin-arg-mark-unused dump-ast -fsyntax-only %s | %filecheck %s
int foo(int a, int b, int c) {
    double value = 0.0;
    return a + b;
}

// CHECK: ParmVarDecl {{.*}} c 'int'
// CHECK-NEXT: UnusedAttr {{.*}} maybe_unused

// CHECK: VarDecl {{.*}} x 'double'
// CHECK-NEXT: UnusedAttr {{.*}} maybe_unused