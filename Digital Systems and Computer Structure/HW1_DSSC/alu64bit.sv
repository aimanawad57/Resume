// 64-bit ALU template
module alu64bit (
    input logic [63:0] a,    // Input bit a
    input logic [63:0] b,    // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic [63:0] s,   // Output S
    output logic cout        // Carry out
);

	logic [64:0] c; //c[0] is the cin 
					//c[64] is the cout (overflow)
    assign c[0] = cin;
	
	genvar i;
    generate
        for (i = 0; i < 64; i++)
		begin
            alu1bit alu1bit_inst (
                .a      (a[i]),
                .b      (b[i]),
                .cin    (c[i]),
                .op     (op),
                .s      (s[i]),
                .cout   (c[i+1])
            );
        end
    endgenerate

	assign cout = c[64];
	
endmodule
