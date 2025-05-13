// Full Adder/Subtractor test bench template
module fas_test;

	logic a_t, b_t, cin_t, a_ns_t, cout_t, s_t;
	
	fas f_adder (.a(a_t), .b(b_t), .cin(cin_t), .a_ns(a_ns_t), .cout(cout_t), .s(s_t));
	
	initial begin
	
	a_t = 0;
	b_t = 0;
	cin_t = 0;
	a_ns_t = 1;
	
	#40
	a_t = 1;
	b_t = 0;
	cin_t = 0;
	a_ns_t = 1;
	
	#40
	a_t = 0;
	b_t = 0;
	cin_t = 0;
	a_ns_t = 1;
	

	end

endmodule
