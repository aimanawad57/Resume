// Full Adder/Subtractor template
module fas (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic a_ns,        // A_nS (add/not subtract) control
    output logic s,          // Output S
    output logic cout        // Carry out
);

parameter Tpdlh = 1;
parameter Tpdhl = 1;

logic a_xnor_b , a_xnor_ns , c_or_b , b_nand_c , temp_bf , temp_f;

XNOR2 #(7,7) x1(.Z(a_xnor_b),.B(b),.A(a));

XNOR2 #(7,7) x2(.Z(s),.B(a_xnor_b),.A(cin));


XNOR2 #(7,7) x3(.Z(a_xnor_ns),.B(a_ns),.A(a));

OR2 #(6,6) o1(.Z(c_or_b) , .B(b) , .A(cin));

NAND2 #(8,8) n2(.Z(temp_bf),.B(c_or_b),.A(a_xnor_ns));

NAND2 #(8,8) n1(.Z(b_nand_c),.B(b),.A(cin));

NAND2 #(8,8) n3(.Z(cout),.B(temp_bf),.A(b_nand_c));

endmodule
