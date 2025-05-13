// 32X32 Multiplier test template
module mult32x32_fast_test;

    logic clk;            // Clock
    logic reset;          // Reset
    logic start;          // Start signal
    logic [31:0] a;       // Input a
    logic [31:0] b;       // Input b
    logic busy;           // Multiplier busy indication
    logic [63:0] product; // Miltiplication product

// Put your code here
// ------------------
	
	mult32x32_fast fast_multiplier(.clk(clk),.reset(reset),.start(start),.a(a),.b(b),.busy(busy),.product(product));
	
	always begin
		#5 clk=~clk;
	end
	
	initial begin
		clk=1;
		reset=0;
		start=0;
		a=0;
		b=0;
		
		#10
		
		reset = 1;
		
		repeat(5) begin
			@(posedge clk);
		end
		
		reset = 0;
		
		a = 326286705; 
		b = 212269294;

		@(posedge clk);
		start = 1;
		@(posedge clk);
		start = 0;
		
		repeat(9) begin //this is waiting for busy to be "etsev"
			@(posedge clk);
		end
		
		a = 48497; 
		b = 63726;
		
		@(posedge clk);
		start = 1;
		@(posedge clk);
		start = 0;
		
		repeat(9) begin //this is waiting for busy to be "etsev"
			@(posedge clk);
		end
		
	end
		
		
		
// End of your code

endmodule
