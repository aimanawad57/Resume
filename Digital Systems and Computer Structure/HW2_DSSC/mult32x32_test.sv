// 32X32 Multiplier test template
module mult32x32_test;

    logic clk;            // Clock
    logic reset;          // Reset
    logic start;          // Start signal
    logic [31:0] a;       // Input a
    logic [31:0] b;       // Input b
    logic busy;           // Multiplier busy indication
    logic [63:0] product; // Miltiplication product


	
	mult32x32 m_mult(
		.clk(clk),
		.reset(reset),
		.start(start),
		.a(a),
		.b(b),
		.busy(busy),
		.product(product)
	);
	
		
	initial begin
		clk = 1'b0;
        reset = 1'b0;
        start = 1'b0;
		
		#5
		reset = 1'b1;
		
		repeat(5) begin //wait 4 clock cycles for reset
		//(the last clock rise will also do reset = 0.
		@(posedge clk);
		end
		
		reset = 1'b0;
		
		a = 212269294; 
		b = 326286705;
		
		@(posedge clk);
		start = 1'b1;
		
		@(posedge clk);
		start = 1'b0;
		
		repeat(9) begin 
		@(posedge clk);
		end
		
	end	
		
    always begin
		#5
		clk = ~clk;
	end
	
endmodule
