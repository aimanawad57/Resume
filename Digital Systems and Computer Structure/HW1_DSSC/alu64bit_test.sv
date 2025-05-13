// 64-bit ALU test bench template
module alu64bit_test;

	logic [63:0] test_a;
	logic [63:0] test_b;
	logic test_cin;
	logic [1:0] test_op;
	logic [63:0] test_s;
	logic test_cout;

	alu64bit uut (
		.a(test_a),
		.b(test_b),
		.cin(test_cin),
		.op(test_op),
		.s(test_s),
		.cout(test_cout)
		);

	initial begin 
		test_a = {64{1'b1}} ;
		test_b = {64{1'b0}};
		test_cin = 0;
		test_op = 2'b10;
		
		#1500
		
		test_a = {64{1'b1}} ;
		test_b = {64{1'b0}};
		test_cin = 1;
		test_op = 2'b10;
		
		#1500
		
		test_a = {64{1'b1}} ;
		test_b = {64{1'b0}};
		test_cin = 0;
		test_op = 2'b10;
	end

endmodule
