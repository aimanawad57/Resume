// 32X32 Multiplier arithmetic unit template
module mult32x32_fast_arith (
    input logic clk,             // Clock
    input logic reset,           // Reset
    input logic [31:0] a,        // Input a
    input logic [31:0] b,        // Input b
    input logic [1:0] a_sel,     // Select one byte from A
    input logic b_sel,           // Select one 2-byte word from B
    input logic [2:0] shift_sel, // Select output from shifters
    input logic upd_prod,        // Update the product register
    input logic clr_prod,        // Clear the product register
    output logic a_msb_is_0,     // Indicates MSB of operand A is 0
    output logic b_msw_is_0,     // Indicates MSW of operand B is 0
    output logic [63:0] product  // Miltiplication product
);




logic[7:0] a_temp;
logic[15:0] b_temp;
logic[23:0] mult_to_shifter;
logic[63:0] shifter_to_adder;
logic[63:0] adder_to_prod;
logic[63:0] prod_to_adder;

// FSM synchronous procedural block for storing product.
    always_ff @(posedge clk, posedge reset) begin
        if (reset == 1'b1 || clr_prod == 1'b1) begin
            product <= 64'b0;
        end
        else if (upd_prod == 1'b1 ) begin
        	product <= adder_to_prod;    
        end
    end
	
	always_comb begin
		//MUX 4->1 for a
		case(a_sel)
			2'b00: begin
				assign a_temp = a[7:0];
			end
			2'b01: begin
				assign a_temp = a[15:8];
			end
			2'b10: begin
				assign a_temp = a[23:16];
			end
			2'b11: begin
				assign a_temp = a[31:24];
			end
		endcase
		
		//MUX 2-> 1 for b
		if(b_sel == 0) begin
			assign b_temp = b[15:0];
		end
		else begin
			assign b_temp = b[31:16];
		end
		
		
	begin	
		if(a[31:24] == 8'b0) begin
			a_msb_is_0 = 1;
		end
		else
			begin
			a_msb_is_0 = 0;
			end
		if(	b[31:16] == 16'b0) begin
			b_msw_is_0 = 1;
			end
		else
			begin
			b_msw_is_0 = 0;
			end
		
	end	
		//the 16x8 Multiplier
		assign mult_to_shifter = a_temp * b_temp;
		
		//MUX 8->1 for the shifters
		case(shift_sel)
		3'b000: begin
			assign shifter_to_adder = mult_to_shifter;
		end
		3'b001: begin
			assign shifter_to_adder = mult_to_shifter << 8;
		end
		3'b010: begin
			assign shifter_to_adder = mult_to_shifter << 16;
		end
		3'b011: begin
			assign shifter_to_adder = mult_to_shifter << 24;
		end
		3'b100: begin
			assign shifter_to_adder = mult_to_shifter << 32;
		end
		3'b101: begin
			assign shifter_to_adder = mult_to_shifter << 40;
		end
		3'b110: begin
			assign shifter_to_adder = 0;
		end
		3'b111: begin
			assign shifter_to_adder = 0;
		end
		endcase
		
		//64bit adder
		prod_to_adder = product;
		assign adder_to_prod = shifter_to_adder + prod_to_adder;
	end
// End of your code

endmodule

