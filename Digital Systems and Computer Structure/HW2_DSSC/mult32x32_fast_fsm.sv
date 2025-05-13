// 32X32 Multiplier FSM
module mult32x32_fast_fsm (
    input logic clk,              // Clock
    input logic reset,            // Reset
    input logic start,            // Start signal
    input logic a_msb_is_0,       // Indicates MSB of operand A is 0
    input logic b_msw_is_0,       // Indicates MSW of operand B is 0
    output logic busy,            // Multiplier busy indication
    output logic [1:0] a_sel,     // Select one byte from A
    output logic b_sel,           // Select one 2-byte word from B
    output logic [2:0] shift_sel, // Select output from shifters
    output logic upd_prod,        // Update the product register
    output logic clr_prod         // Clear the product register
);

// Put your code here
// ------------------

//our FSM states (m0 is the idle state)
typedef enum {m0, m1, m2, m3, m4, m5, m6, m7, m8} sm_type;

sm_type current_state;
sm_type next_state;

//FSM synchronous procedural block
always_ff @(posedge clk, posedge reset) begin
	if(reset == 1'b1) begin
		current_state <= m0;
	end
	else begin
		current_state <= next_state;
	end
end

always_comb begin
	next_state = current_state;
	busy = 0;
	a_sel = 0;
	b_sel = 0;
	shift_sel = 0;
	upd_prod = 0;
	clr_prod = 0;
	
case(current_state)
	m0: begin
		if(start == 1) begin
			next_state = m1;
			clr_prod = 1;
			//Do we need to do busy = 1 ??
			//shift_sel = 111 will help ? 
		end
	end
	
	m1: begin
		next_state = m2;
		busy = 1;
		a_sel = 0;
		b_sel = 0;
		shift_sel = 0;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m2: begin
		next_state = m3;
		busy = 1;
		a_sel = 1;
		b_sel = 0;
		shift_sel = 1;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m3: begin
		if (a_msb_is_0 == 1 && b_msw_is_0 == 1) begin
			next_state = m0;
		end else if (a_msb_is_0 == 1) begin
			next_state = m5;
		end else begin
			next_state = m4;
		end 

		
		busy = 1;
		a_sel = 2;
		b_sel = 0;
		shift_sel = 2;
		upd_prod = 1;
		clr_prod = 0;
	end	

	
	m4: begin
		if(b_msw_is_0 == 1) begin
			next_state = m0;
		end
		else begin
			next_state = m5;
		end
		busy = 1;
		a_sel = 3;
		b_sel = 0;
		shift_sel = 3;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m5: begin
		next_state = m6;
		busy = 1;
		a_sel = 0;
		b_sel = 1;
		shift_sel = 2;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m6: begin 
		next_state = m7;
		busy = 1;
		a_sel = 1;
		b_sel = 1;
		shift_sel = 3;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m7: begin
		if(a_msb_is_0 == 1) begin
			next_state = m0;
		end
		else begin
			next_state = m8;
		end
		busy = 1;
		a_sel = 2;
		b_sel = 1;
		shift_sel = 4;
		upd_prod = 1;
		clr_prod = 0;
	end
	
	m8: begin
		next_state = m0;
		busy = 1;
		a_sel = 3;
		b_sel = 1;
		shift_sel = 5;
		upd_prod = 1;
		clr_prod = 0;
	end
	endcase
end
	
// End of your code

endmodule
