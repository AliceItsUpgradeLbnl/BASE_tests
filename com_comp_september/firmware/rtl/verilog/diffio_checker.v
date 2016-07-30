//=============================================================================
// Title: Differential error bit checker
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/22/2016
// Last modified: 7/28/2016 (AC)
// Description: performs a full check of the 8 differential IO of differential
//              to single ended driver SN65MLVD080DGG 
//=============================================================================

module diffio_checker #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency
  parameter BASE_FREQUENCY = 125000           // Min frequency of state-to-state transition of daughter state machines (2 * diffIO test frequency)
)
(
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset
  
  // Start/busy interface (from/to transaction state machine)
  input  START,                             
  input  [4:0] FREQ_SCALE_FACTOR,           
  output BUSY,                       

  // Differential buffers data/control signals
  input   [7:0] DIFF_IN,
  output  [7:0] DIFF_OEN,
  output  [7:0] DIFF_OUT,

  // Bit pattern errors detected during the data transmission 
  output  [255:0] ERROR_COUNTERS,
  output  [7:0]   ERROR_FLAGS
);

//========================================================================
//                            Local parameters 
//========================================================================

localparam SEED = 32'hABCDEF01; // Used by pseudo-random bit pattern generator

//========================================================================
//                            Internal Signals 
//========================================================================

wire clk_en;

wire [7:0] bit_pattern_generated;
wire [7:0] bit_pattern_to_check;
wire [7:0] busy_pattern_checker;
wire [7:0] busy_pattern_generator;
wire start_even_channels, start_odd_channels;
wire busy_even_channels, busy_odd_channels;

wire [31:0] error_counter [7:0];

//========================================================================
//                          Combinatorial logic 
//========================================================================

assign ERROR_COUNTERS = {error_counter[7],error_counter[6],error_counter[5],error_counter[4],
                         error_counter[3],error_counter[2],error_counter[1],error_counter[0]};

assign ERROR_FLAGS = {|error_counter[7],|error_counter[6],|error_counter[5],|error_counter[4],
                      |error_counter[3],|error_counter[2],|error_counter[1],|error_counter[0]}; 

assign busy_even_channels = busy_pattern_generator[0] | busy_pattern_generator[2] | busy_pattern_generator[4] | busy_pattern_generator[6] |
                            busy_pattern_checker[0]   | busy_pattern_checker[2]   | busy_pattern_checker[4]   | busy_pattern_checker[6];

assign busy_odd_channels  = busy_pattern_generator[1] | busy_pattern_generator[3] | busy_pattern_generator[5] | busy_pattern_generator[7] |
                            busy_pattern_checker[1]   | busy_pattern_checker[3]   | busy_pattern_checker[5]   | busy_pattern_checker[7];

//========================================================================
//                            Internal Modules 
//========================================================================

// Coordinates generators and checkers
diffio_checker_main_sm diffio_checker_main_sm(
  // Clock interface
  .CLK(CLK),                         
  .RST_N(RST_N),                    

  // Start/busy interface from outside the Differential IO checker
  .START(START),
  .BUSY(BUSY),

  // Start/busy signals from/to other daughter state machines
  .BUSY_EVEN(busy_even_channels),
  .BUSY_ODD(busy_odd_channels),
  .DATA_TO_SEND(bit_pattern_generated), 
  .DATA_TO_CHECK(bit_pattern_to_check), 
  .START_EVEN_CHANNELS(start_even_channels),
  .START_ODD_CHANNELS(start_odd_channels),
  
  // Differential buffers data/control signals
  .DIFF_IN(DIFF_IN),
  .DIFF_OEN(DIFF_OEN),
  .DIFF_OUT(DIFF_OUT) 
);

// Determines frequency of generators and checkers sm (user defined via FREQ_SCALE_FACTOR) 
diffio_checker_clock_divider #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY),
  .BASE_FREQUENCY(BASE_FREQUENCY)
)
diffio_checker_clock_divider(
  .CLK(CLK),             // System clock
  .RST_N(RST_N),         // Active low reset
  .FREQ_SCALE_FACTOR(FREQ_SCALE_FACTOR),
  .CLK_EN(clk_en)        // Clock enable (transitions of daughter state machines allowed when high)
);

genvar i;

// Creating bit pattern generators and checkers (daughter state machines)

generate for (i = 0; i < 4; i = i + 1) begin: diffio_gen_and_check

  //------------------- Generators on even channels and checkers on odd channels ----------------------

  diffio_pattern_generator_sm #(
    .NUM_BITS_TO_SEND(1000),
    .SEED(SEED + 2*i*10)
  )
  diffio_pattern_generator_sm_even(
    // Clock interface
    .CLK(CLK),                         
    .CLK_EN(clk_en),                     
    .RST_N(RST_N),                 
  
    // Start/busy interface to main sm
    .START(start_even_channels),  
    .BUSY(busy_pattern_generator[2*i]),
  
    // Pseudo-random pattern (generated)
    .BIT_PATTERN(bit_pattern_generated[2*i])
  );
	 
  diffio_pattern_checker_sm #(
    .NUM_BITS_TO_CHECK(1000),
    .SEED(SEED + 2*i*10)
  )
  diffio_pattern_checker_sm_odd(
    // Clock interface
    .CLK(CLK),                          
    .CLK_EN(clk_en),                    
    .RST_N(RST_N),                    
 
    // Start/busy interface to main sm
    .START(start_even_channels),          
    .BUSY(busy_pattern_checker[2*i + 1]), 
  
    // Pseudo-random pattern (to check) and detected errors
    .BIT_PATTERN(bit_pattern_to_check[2*i + 1]),
    .ERROR_COUNTER(error_counter[2*i + 1])
  );
	 
  //------------------- Generators on odd channels and checkers on even channels ----------------------
	 
  diffio_pattern_generator_sm #(
    .NUM_BITS_TO_SEND(1000),
    .SEED(SEED + (2*i + 1)*10)
  )
  diffio_pattern_generator_sm_odd(
    // Clock interface
    .CLK(CLK),                 
    .CLK_EN(clk_en),                    
    .RST_N(RST_N),              
 
    // Start/busy interface to main sm
    .START(start_odd_channels),           
    .BUSY(busy_pattern_generator[2*i + 1]),
 
    // Pseudo-random pattern (generated)
    .BIT_PATTERN(bit_pattern_generated[2*i + 1])
  );
	 
  diffio_pattern_checker_sm #(
    .NUM_BITS_TO_CHECK(1000),
    .SEED(SEED + (2*i + 1)*10)
  )
  diffio_pattern_checker_sm_even(
    // Clock interface
    .CLK(CLK),                          
    .CLK_EN(clk_en),                 
    .RST_N(RST_N),                       
 
    // Start/busy interface to main sm
    .START(start_odd_channels),           
    .BUSY(busy_pattern_checker[2*i]),     

    // Pseudo-random pattern (to check) and detected errors
    .BIT_PATTERN(bit_pattern_to_check[2*i]),
    .ERROR_COUNTER(error_counter[2*i])
  );

end endgenerate

endmodule
