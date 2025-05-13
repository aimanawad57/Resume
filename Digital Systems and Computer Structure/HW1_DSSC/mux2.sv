// 2->1 multiplexer template
module mux2 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic sel,         // Select input
    output logic z           // Output
);

logic temp1 , temp2 , sel_n;




NAND2 #(8, 8) u1(.Z(temp1),.A(sel), .B(d1));
NAND2 #(8, 8) u2(.Z(sel_n), .A(sel),.B(sel));
NAND2 #(8, 8) u3(.Z(temp2), .A(sel_n), .B(d0));
NAND2 #(8, 8) u4(.Z(z), .A(temp2), .B(temp1));





endmodule
