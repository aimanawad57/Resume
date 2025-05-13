// 1-bit ALU template
module alu1bit (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic s,          // Output S
    output logic cout        // Carry out
);

parameter Tpdlh = 1;
parameter Tpdhl = 1;

logic nor_result, xor_result, a_or_b, a_xnor_b, a_ns, add_sub;

OR2 #(6,6) g1 (.Z(a_or_b), .B(b), .A(a));

NAND2 #(8,8) g2 (.Z(nor_result), .B(a_or_b), .A(a_or_b));

XNOR2 #(7,7) g3 (.Z(a_xnor_b), .B(b), .A(a));

NAND2 #(8,8) g4 (.Z(xor_result), .B(a_xnor_b), .A(a_xnor_b));

NAND2 #(8,8) g5 (.Z(a_ns), .B(op[0]), .A(op[0]));

fas #(50,50) g6 (.a(a), .b(b), .cin(cin), .a_ns(a_ns), .s(add_sub), .cout(cout));

mux4 #(36,36) g7 (.d0(nor_result), .d1(xor_result), .d2(add_sub), .d3(add_sub), .sel(op), .z(s));



endmodule
