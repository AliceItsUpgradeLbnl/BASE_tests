//=============================================================================
// Title: Transactions state machine
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/17/2016
// Last modified: 7/22/2016 (AC)
// Description: this can initiate transactions towards the electronics built for
//              3/2016 radiation tests
//=============================================================================

module transactions_sm (

  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset
  
  // Start/CMD/busy interface
  input  [31:0] CMD,                          // Command to be executed (includes number of bits to be transmitted) 
  input  START,                               // Start an operation
  output reg BUSY,                            // Indicates transaction in progress

  // Signals to external SPI/I2C interfaces (either on ADC-DAC board and DUT)
  input  [31:0] BUSY_ADC,
  input  [7:0]  BUSY_DAC,
  input  [7:0]  BUSY_WF,
  input  BUSY_DIFFIO_CHECKER,
  input  BUSY_INTERRUPT_CHECKER,
  input  BUSY_DP,
  input  BUSY_I2C_DUT,
  input  BUSY_I2C_DUT_AUX,
  input  BUSY_SPI_DUT,
  output [31:0] START_ADC,
  output [7:0]  START_DAC,
  output [7:0]  START_WF,
  output START_DIFFIO_CHECKER,
  output START_INTERRUPT_CHECKER,
  output START_DP,
  output START_I2C_DUT,
  output START_I2C_DUT_AUX,
  output START_SPI_DUT,
  output reg [1:0] OP_TYPE_SPI,
   
  // Signals to the state machine that sends data to PC
  input  BUSY_SEND,
  output reg SEND_DATA,
  output reg SEND_HEADER,
  output reg SEND_TRAILER
);

//========================================================================
//                            Internal Signals 
//========================================================================

wire [7:0] delay;
wire busy_time;
wire [2:0] dev_type;
wire [3:0] op_type;
wire [7:0] num_of_samples;
wire [1:0] num_of_bytes_to_rec, num_of_bytes_to_transm;
wire dev_is_adc;

reg  last_sample;
reg  inc_samp_counter;
reg  [7:0] optype_decoded;
reg  rst_samp_counter;
reg  [9:0] samp_counter;
reg  start_int;
reg  [9:0] time_counter;

reg  [3:0] sm_state;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 4'b0000;
localparam CHECKOP = 4'b0001;
localparam SENDHEADER = 4'b0010;
localparam WAITHEADERSENT = 4'b0011;
localparam STARTINT = 4'b0100;
localparam WAITINTEND = 4'b0101;
localparam SENDDATA = 4'b0110;
localparam WAITDATASENT = 4'b0111;
localparam SENDTRAILER = 4'b1000;
localparam WAITTRAILERSENT = 4'b1001;

//========================================================================
//                            State machine core 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end else begin
    case (sm_state)
      IDLE: begin
	if (START)
          sm_state   <= CHECKOP;
        else
          sm_state   <= IDLE;		  
      end
      CHECKOP: begin
        if (read_operation && !exception)
          sm_state   <= SENDHEADER;          
        else
          sm_state   <= STARTINT;	  
      end
      SENDHEADER: begin
        sm_state   <= WAITHEADERSENT;  
      end
      WAITHEADERSENT: begin
	if (!BUSY_SEND)
          sm_state   <= STARTINT;
        else
          sm_state   <= WAITHEADERSENT;   
      end
      STARTINT: begin
	if (busy_int)      
          sm_state   <= WAITINTEND;
        else
          sm_state   <= STARTINT;
      end
      WAITINTEND: begin
	   if (busy_ext)
          sm_state   <= WAITINTEND;
        else if (read_operation)
          sm_state   <= SENDDATA;
        else
          sm_state   <= IDLE;
      end
      SENDDATA: begin
        if (dev_is_adc && !last_sample)
          sm_state   <= STARTINT;
        else
          sm_state   <= WAITDATASENT;
      end
      WAITDATASENT: begin
        if (BUSY_SEND)
          sm_state   <= WAITDATASENT;
        else begin
          if (!exception)
            sm_state   <= SENDTRAILER;
          else 
            sm_state   <= IDLE;
        end
      end
      SENDTRAILER: begin
        sm_state     <= WAITTRAILERSENT;
      end
      WAITTRAILERSENT: begin
	if (BUSY_SEND)
          sm_state   <= WAITTRAILERSENT;
        else
          sm_state   <= IDLE;
      end
      default: begin
        sm_state     <= IDLE;
      end
    endcase
  end
end

//========================================================================
//                      State machine output signals 
//========================================================================

always@(*) begin
  case (sm_state)
    IDLE: begin
      BUSY                = 1'b0;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    CHECKOP: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    SENDHEADER: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b1;
      SEND_TRAILER        = 1'b0;
    end
    WAITHEADERSENT: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    STARTINT: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b1;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    WAITINTEND: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    SENDDATA: begin
      BUSY                = 1'b1;
      start_int           = 1'b0;
      SEND_DATA           = 1'b1;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
      if (dev_is_adc && !last_sample) begin
        inc_samp_counter  = 1'b1;
        rst_samp_counter  = 1'b0;
      end else begin
        inc_samp_counter  = 1'b0;
        rst_samp_counter  = 1'b1;
      end
    end
    WAITDATASENT: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    SENDTRAILER: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b1;
    end
    WAITTRAILERSENT: begin
      BUSY                = 1'b1;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
    default: begin
      BUSY                = 1'b0;
      inc_samp_counter    = 1'b0;
      rst_samp_counter    = 1'b0;
      start_int           = 1'b0;
      SEND_DATA           = 1'b0;
      SEND_HEADER         = 1'b0;
      SEND_TRAILER        = 1'b0;
    end
  endcase
end

//========================================================================
//                         Combinatorial Logic
//========================================================================

// Reminder
// CMD[31:24] = minimum operation time for the ADCs ((2*this value + 4) 50MHz clk cycles if this value is equal-higher than 10 else it is 24)
// CMD[23:16] = num of samples for the ADCs (this value + 1)
// CMD[15:11] = frequency scale factor for the diffio checker (this field*20kHz) - used outside 
// CMD[11]    = SCLK_EN_PHASE - Used outside
// CMD[10:8]  = dev_type
// CMD[7:4]   = op_type
// CMD[3:2]   = num_bytes_to_receive (not used in this module)
// CMD[1:0]   = num_bytes_to_transmit (not used in this module)

assign delay = CMD[31:24];
assign num_of_samples = CMD[23:16];
assign dev_type = CMD[10:8];
assign op_type = CMD[7:4];
assign num_of_bytes_to_rec = CMD[3:2];
assign num_of_bytes_to_transm = CMD[1:0];
assign read_operation = ((dev_type == 3'd0) || (((dev_type == 3'd3) || (dev_type == 3'd4) || (dev_type == 3'd5) || (dev_type == 3'd6)) && |num_of_bytes_to_rec) || (dev_type == 3'd7 && op_type == 4'd0));
assign exception = ((dev_type == 3'd4) || (dev_type == 3'd5) || (dev_type == 3'd6));

always @(*) begin
  if ((num_of_bytes_to_rec == 2'd0) && (num_of_bytes_to_transm != 2'd0))
    OP_TYPE_SPI = 2'd0;
  else if ((num_of_bytes_to_rec != 2'd0) && (num_of_bytes_to_transm != 2'd0))
    OP_TYPE_SPI = 2'd1;
  else if ((num_of_bytes_to_rec != 2'd0) && (num_of_bytes_to_transm == 2'd0))
    OP_TYPE_SPI = 2'd2;
  else
    OP_TYPE_SPI = 2'd3;
end

assign dev_is_adc = (dev_type == 3'd0);

always @(*) begin
  if (num_of_samples != 8'hFF)
    last_sample = (samp_counter == {2'd0,num_of_samples});
  else
    last_sample = (samp_counter == 10'd999);
end

assign busy_int = |{BUSY_ADC,BUSY_DAC,BUSY_WF,BUSY_DIFFIO_CHECKER,BUSY_INTERRUPT_CHECKER,BUSY_DP,BUSY_I2C_DUT,BUSY_I2C_DUT_AUX,BUSY_SPI_DUT};
assign busy_ext = |{BUSY_ADC,BUSY_DAC,BUSY_WF,BUSY_DIFFIO_CHECKER,BUSY_INTERRUPT_CHECKER,BUSY_DP,BUSY_I2C_DUT,BUSY_I2C_DUT_AUX,BUSY_SPI_DUT} || busy_time;

// List of implemented commands:
// dev_type = 3'd0 (SPI ADCs on ADC-DAC board, channels can be started in groups of 8)
// -- op_type = {start_adc_ch_31-24, start_adc_ch_23-16, start_adc_ch_15-8, start_adc_ch_7-0}
// dev_type = 3'd1 (SPI DACs on ADC-DAC board, each DAC chip drives 4 DAC channels)
// -- op_type = 4'd0 -> start_dac_0 (drives dac channels 3-0) 
// -- op_type = 4'd1 -> start_dac_1 (drives dac channels 7-4) 
// -- op_type = 4'd2 -> start_dac_2 (drives dac channels 11-8) 
// -- op_type = 4'd3 -> start_dac_3 (drives dac channels 15-12) 
// -- op_type = 4'd4 -> start_dac_4 (drives dac channels 19-16) 
// -- op_type = 4'd5 -> start_dac_5 (drives dac channels 23-20) 
// -- op_type = 4'd6 -> start_dac_6 (drives dac channels 27-24) 
// -- op_type = 4'd7 -> start_dac_7 (drives dac channels 31-28)
// -- op_type = 4'd8 -> start_all_dacs (broadcast command to the 8 DAC chips). Any number bigger than 8 is also broadcast
// -- op_type = others -> unused
// dev_type = 3'd2 (SPI Waveform Generators on ADC-DAC board, each driving a channel)
// -- op_type = 4'd0 -> start_wf_0
// -- op_type = 4'd1 -> start_wf_1
// -- op_type = 4'd2 -> start_wf_2
// -- op_type = 4'd3 -> start_wf_3
// -- op_type = 4'd4 -> start_wf_4
// -- op_type = 4'd5 -> start_wf_5
// -- op_type = 4'd6 -> start_wf_6 
// -- op_type = 4'd7 -> start_wf_7
// -- op_type = 4'd8 -> start_all_wf (broadcast command to the 8 WF chips). Any number bigger than 8 is also broadcast
// -- op_type = others -> unused
// dev_type = 3'd3 (I2C Digital potentiometers on ADC-DAC board, 4 chips)
// dev_type = 3'd4 (I2C devices on DUT board)
// dev_type = 3'd5 (SPI devices on ADC-DAC board, 2 chips)
// dev_type = 3'd6 (I2C devices on DUT board accessed via auxiliary I2C link)
// dev_type = 3'd7 (other operations):
// -- op_type = 4'd0 -> Start differential IO check
// -- op_type = d'd1 -> Start Interrupt check
// Notice that, in the case dev_type = 3'd5, CMD[6:7] i.e. op_type[2:3] is used externally to allow single or simultaneous communication with the DUT SPI devices 

always @(*) begin
  case (op_type)
    4'd0:
      optype_decoded = 8'b00000001;
    4'd1:
      optype_decoded = 8'b00000010;
    4'd2:
      optype_decoded = 8'b00000100;
    4'd3:
      optype_decoded = 8'b00001000;
    4'd4:
      optype_decoded = 8'b00010000;
    4'd5:
      optype_decoded = 8'b00100000;
    4'd6:
      optype_decoded = 8'b01000000;
    4'd7:
      optype_decoded = 8'b10000000;
    default:
      optype_decoded = 8'b00000000;
  endcase
end

assign START_ADC = {32{start_int}} & {32{dev_type == 3'd0}} & {{8{op_type[3]}}, {8{op_type[2]}}, {8{op_type[1]}}, {8{op_type[0]}}};
assign START_DAC = {8{start_int}} & {8{dev_type == 3'd1}} & (optype_decoded | {8{op_type[3]}}); 
assign START_WF  = {8{start_int}} & {8{dev_type == 3'd2}} & (optype_decoded | {8{op_type[3]}}); 
assign START_DP  = start_int && (dev_type == 3'd3); 
assign START_I2C_DUT = start_int && (dev_type == 3'd4);
assign START_SPI_DUT = start_int && (dev_type == 3'd5);
assign START_I2C_DUT_AUX = start_int && (dev_type == 3'd6);
assign START_DIFFIO_CHECKER = start_int && (dev_type == 3'd7) && (op_type == 4'd0);
assign START_INTERRUPT_CHECKER = start_int && (dev_type == 3'd7) && (op_type == 4'd1);

//========================================================================
//            Counter of the number of samples for the ADCs
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    samp_counter   <= 8'd0;
  else if (rst_samp_counter)
    samp_counter   <= 8'd0;
  else if (inc_samp_counter)
    samp_counter   <= samp_counter + 1'b1;
end

//========================================================================
//          Counter of the minimum operation time for the ADCs
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    time_counter   <= 8'd0;
  else if (start_int)
    time_counter   <= 8'd1;
  else if (((time_counter == 2'd2*delay + 10'd4) && (delay >= 8'd10)) || ((time_counter == 10'd24) && (delay < 8'd10)))
    time_counter   <= 8'd0;  
  else if (time_counter != 8'd0)
    time_counter   <= time_counter + 1'b1;
end

assign busy_time = (time_counter != 8'd0);

endmodule