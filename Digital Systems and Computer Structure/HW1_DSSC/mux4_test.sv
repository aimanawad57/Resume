// 4->1 multiplexer test bench template
module mux4_test;
	logic D0;
	logic D1;
	logic D2;
	logic D3;
	logic [1:0] SEL;
	logic Z;
	
	mux4 m4 (.d0(D0),.d1(D1),.d2(D2),.d3(D3),.sel(SEL),.z(Z));
	
	initial begin
		D0 = 0;
		D1 = 1;
		D2 = 0;
		D3 = 0;
		SEL[0] = 0;
		SEL[1] = 0;
		
		#50
		
		D0 = 0;
		D1 = 1;
		D2 = 0;
		D3 = 0;
		SEL[0] = 1;
		SEL[1] = 0;
		
		#50
			
		D0 = 0;
		D1 = 1;
		D2 = 0;
		D3 = 0;
		SEL[0] = 0;
		SEL[1] = 0;

	end
	

endmodule
