//=============================================================================
// Title: I2C registers
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/30/2015
// Last modified: 10/30/2015 (AC)
// Description: this module is used to register the I2C interface incoming and
//              outgoing data and control signals from an external control interface
//=============================================================================

module i2c_regs (
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset

  // Control signals
  input      LATCH_INPUT_DATA,                // Latches the data from external interface to a internal register

  // Input data interface          
  input      [55:0] I2C_DATA_TO_TRANSM,       // I2C data to transmit to slave device - unregistered 
  input      [6:0]  SL_ADDR,                  // I2C slave address - unregistered
  output reg [63:0] DATA_FROM_CORE,           // Data to send to slave device - registered

  // Size of data to transm and receive
  input      [4:0] NUM_BYTES_TO_REC_IN,       // Number of bytes to receive from slave device - unregistered
  output reg [4:0] NUM_BYTES_TO_REC_OUT,      // Number of bytes to receive from slave device - registered
  input      [2:0] NUM_BYTES_TO_TRANSM_IN,    // Number of bytes to transmit to slave device (excluded the slave address byte) - unregistered
  output reg [2:0] NUM_BYTES_TO_TRANSM_OUT,   // Number of bytes to transmit to slave device (excluded the slave address byte) - registered

  // Control for internal state machines 
  input      REP_START_IN,                    // Repeate start - unregistered
  output reg REP_START_OUT,                   // Repeate start - registered
  output reg RW                               // Type of operation - registered
);

//========================================================================
//                           Internal Signals 
//========================================================================

reg  [6:0]  sl_addr_reg;
reg  [55:0] int_data_reg; 

//========================================================================
//                           Input registers
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    sl_addr_reg              <= 7'd0;
  else if (LATCH_INPUT_DATA)
    sl_addr_reg              <= SL_ADDR;
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    int_data_reg             <= 56'd0;
  else if (LATCH_INPUT_DATA)
    int_data_reg             <= I2C_DATA_TO_TRANSM;
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    REP_START_OUT            <= 1'b0;
  else if (LATCH_INPUT_DATA)
    REP_START_OUT            <= REP_START_IN;
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    RW                       <= 1'b0;
  else if (LATCH_INPUT_DATA)
    RW                       <= |(NUM_BYTES_TO_REC_IN);
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    NUM_BYTES_TO_TRANSM_OUT  <= 3'd0;
  else if (LATCH_INPUT_DATA)
    NUM_BYTES_TO_TRANSM_OUT  <= NUM_BYTES_TO_TRANSM_IN;
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    NUM_BYTES_TO_REC_OUT     <= 5'd0;
  else if (LATCH_INPUT_DATA)
    NUM_BYTES_TO_REC_OUT     <= NUM_BYTES_TO_REC_IN;
end

//========================================================================
//                       Output combinatorial logic
//========================================================================

always @(*) begin
  DATA_FROM_CORE[7:0]    = {sl_addr_reg, RW && !REP_START_OUT};
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=1)
    DATA_FROM_CORE[15:8] = int_data_reg[7:0];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 0) && REP_START_OUT)
    DATA_FROM_CORE[15:8] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[15:8] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=2)
    DATA_FROM_CORE[23:16] = int_data_reg[15:8];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 1) && REP_START_OUT)
    DATA_FROM_CORE[23:16] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[23:16] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=3)
    DATA_FROM_CORE[31:24] = int_data_reg[23:16];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 2) && REP_START_OUT)
    DATA_FROM_CORE[31:24] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[31:24] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=4)
    DATA_FROM_CORE[39:32] = int_data_reg[31:24];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 3) && REP_START_OUT)
    DATA_FROM_CORE[39:32] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[39:32] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=5)
    DATA_FROM_CORE[47:40] = int_data_reg[39:32];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 4) && REP_START_OUT)
    DATA_FROM_CORE[47:40] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[47:40] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT >=6)
    DATA_FROM_CORE[55:48] = int_data_reg[47:40];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 5) && REP_START_OUT)
    DATA_FROM_CORE[55:48] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[55:48] = 8'd0;
end

always @(*) begin
  if (NUM_BYTES_TO_TRANSM_OUT ==7)
    DATA_FROM_CORE[63:56] = int_data_reg[55:48];
  else if ((NUM_BYTES_TO_TRANSM_OUT == 6) && REP_START_OUT)
    DATA_FROM_CORE[63:56] = {sl_addr_reg, RW};
  else
    DATA_FROM_CORE[63:56] = 8'd0;
end

endmodule
