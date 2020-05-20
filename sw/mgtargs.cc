#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include "mgtargs.h"

void mgt_arg_help() { 
  printf("\n");
  printf("mgttool usage: mgttool --long_option=<option value> or -short_option<option value>\n");
  printf("--------------------------------------------------------------------------------\n");
  printf("Required Options:\n");
  printf("\t--device(=)<device> or -d<device> , eg: -dapollo.c2c.vu7p\n"); 
  printf("\t--quad(=)<quad> or -q<quad> , eg: -q11\n"); 
  printf("Optional Options:\n");
  printf("\t--reset(=)<0,1,empty> or -r<0,1,empty> :  Empty argument toggles. Global reset\n"); 
  printf("\t--txusrrst(=)<0,1,empty>. Empty argument toggles : Defaults to all channels\n"); 
  printf("\t--rxusrrst(=)<0,1,empty>. Empty argument toggles : Defaults to all channels\n"); 
  printf("\t--reset_crc_counters(=)<0,1,empty> : Empty argument toggles. Defaults to all channels\n"); 
  printf("\t--loopback(=)<mode> or -l<mode> :  Defaults to all channels\n"); 
  printf("\t--txpolarity(=)<mode> :  Defaults to all channels\n"); 
  printf("\t--rxpolarity(=)<mode> :  Defaults to all channels\n"); 
  printf("\t--rxlpm(=)<mode> :  Defaults to all channels\n"); 
  printf("\t--channel(=)<value> or -c<value> : A channel ID (0-3) for play/capture, a bitmask (0x1-0xf) otherwise \n"); 
  printf("\t--bufftype(=)<value> or -b<value> : Buffer type for play/capture, 'tx' or 'rx' \n"); 
  printf("\t--play(=)<prefix> or -p<prefix>: Requires channel spec.  Prefix added to MS nibble of each 16b word \n"); 
  printf("\t\t--file(=)<filename> or -f<filename>: optional filename for play\n"); 
  printf("\t--capture : Requires channel specification\n"); 
  printf("\t--dump or -v : Dumps the MGT status\n"); 
  printf("\n");
}

int mgt_parse_args( int argc, char ** argv, DevStruct & dev ) {

  int opt;
  int option_index = 0;
  sprintf(dev.device,"");

static struct option long_options[] =
  {
    {"txusrrst",            optional_argument, NULL, 0},
    {"rxusrrst",            optional_argument, NULL, 0},
    {"reset_crc_counters",  optional_argument, NULL, 0},
    {"play",                optional_argument, NULL, 'p'},
    {"file",                required_argument, NULL, 'f'},
    {"capture",             no_argument,       NULL, 0},
    {"help",                no_argument,       NULL, 'h'},
    {"device",              required_argument, NULL, 'd'},
    {"quad",                required_argument, NULL, 'q'},
    {"channel" ,            required_argument, NULL, 'c'},
    {"bufftype" ,           required_argument, NULL, 'b'},
    {"loopback",            required_argument, NULL, 'l'},
    {"ttcext",              optional_argument, NULL, 'x'},
    {"txpolarity",          required_argument, NULL, 0},
    {"rxpolarity",          required_argument, NULL, 0},
    {"rxlpm",               required_argument, NULL, 0},
    {"reset",               optional_argument, NULL, 'r'},
    {"dump",                no_argument,       NULL, 'v'},
    {"align",               required_argument, NULL, 'a'},
    {NULL, 0, NULL, 0}
  };


  while(1)
    {  
      opt = getopt_long(argc, argv, ":d::q::c::f::b::l::r::p::x::a::vh0::",long_options, &option_index);

      if (opt == -1) 
	break; 

      switch(opt)  
        {  
	case 0: 
	  printf("option -->%s\n", long_options[option_index].name); 
	  /* if(!strcmp(long_options[option_index].name,"play") ) { */
	  /*   printf("play: %s\n", optarg);   */
	  /*   dev.mode=MGT_MODE_PLAY; */
	  /*   if(optarg) dev.prefix = (strtoul(optarg,NULL,16))<<12U; */
	  /*   else dev.prefix=0;  */
	  /*   printf("\tprefix value: 0x%x\n",dev.prefix); */
	  /* } */
	  if(!strcmp(long_options[option_index].name,"capture") ) {
	    printf("capture: \n");  
	    dev.mode=MGT_MODE_CAPTURE;
	  }
	  if(!strcmp(long_options[option_index].name,"txusrrst") ) {
	    printf("txusrrst: %s\n", optarg);  
	    if(optarg) dev.tx_usrrst = strtoul(optarg,NULL,10);
	    else       dev.tx_usrrst = MGT_RESET_TOGGLE;
	  }
	  if(!strcmp(long_options[option_index].name,"rxusrrst") ) {
	    printf("rxusrrst: %s\n", optarg);  
	    if(optarg) dev.rx_usrrst = strtoul(optarg,NULL,10);
	    else       dev.rx_usrrst = MGT_RESET_TOGGLE;
	  }
	  if(!strcmp(long_options[option_index].name,"reset_crc_counters") ) {
	    printf("reset_crc_counters: %s\n", optarg);  
	    if(optarg) dev.reset_crc_counters = strtoul(optarg,NULL,10);
	    else       dev.reset_crc_counters = MGT_RESET_TOGGLE;
	  }
	  if(!strcmp(long_options[option_index].name,"txpolarity") ) {
	    if(optarg==NULL) {
	      fprintf(stderr,"Error: txpolarity mode must be specified with  --txpolarity(=)<mode>\n");
	      return 1;
	    }
	    printf("txpolarity: %s\n", optarg);  
	    if(optarg) dev.tx_polarity = strtoul(optarg,NULL,10);
	  }
	  if(!strcmp(long_options[option_index].name,"rxpolarity") ) {
	    if(optarg==NULL) {
	      fprintf(stderr,"Error: txpolarity mode must be specified with  --txpolarity(=)<mode>\n");
	      return 1;
	    }
	    printf("rxpolarity: %s\n", optarg);  
	    if(optarg) dev.rx_polarity = strtoul(optarg,NULL,10);
	  }
	  if(!strcmp(long_options[option_index].name,"rxlpm") ) {
	    printf("rxlpm: %s\n", optarg);  
	    if(optarg) dev.rx_lpm = strtoul(optarg,NULL,10);
	  }
	  break; 
	case 'd':  
	  printf("device: %s\n", optarg);  
	  if(optarg==NULL) {
	    fprintf(stderr,"Error: device must be specified with -d<device> or --device(=)<device>\n");
	    return 1;
	  }
	  sprintf(dev.device,"%s",optarg); // vu7p or ku15p
	  break;  
	case 'q':  
	  printf("quad: %s\n", optarg);  
	  if(optarg==NULL) {
	    fprintf(stderr,"Error: quad must be specified with -q<quad> or --quad(=)<quad>\n");
	    return 1;
	  }
	  dev.quad_id = strtoul(optarg,NULL,10);
	  break;  
	case 'c':  
	  printf("channel: %s\n", optarg);  
	  if(optarg==NULL) {
	    fprintf(stderr,"Error: channel mask must be specified with -c<mask> or --channel(=)<mask>\n");
	    return 1;
	  }
	  dev.channel = strtoul(optarg,NULL,16);
	  break;  
	case 'f':  
	  printf("file: %s\n", optarg);  
	  if(optarg==NULL) {
	    fprintf(stderr,"Error: filename must be specified with -f<filename> or --file(=)<filename>\n");
	    return 1;
	  }
	  dev.filename = std::string(optarg);
	  break;  
	case 'b':  
	  printf("bufftype: %s\n", optarg);  
	  if(optarg==NULL || (strcmp(optarg,"tx") && strcmp(optarg,"rx") ) ) {
	    fprintf(stderr,"Error: bufftype mask must be specified with -b<'tx'|'rx'> or --bufftype(=)<'tx'|'rx'>\n");
	    return 1;
	  }
          if( !strcmp(optarg,"tx") ) dev.bufftype=1;
	  else dev.bufftype=0;
	  break;  
	case 'l':  
	  printf("loopback: %s\n", optarg);  
	  if(optarg==NULL) {
	    fprintf(stderr,"Error: loopback mode must be specified with -l<mode> or --loopback(=)<mode>\n");
	    return 1;
	  }
	  dev.loopback = strtoul(optarg,NULL,10);
	  break;  
// 	case 'x':  
// 	  printf("ttcext: %s\n", optarg);  
// 	  if(optarg==NULL) {
// 	    dev.ttcext = 1;
// 	  } else {
// 	    dev.ttcext = strtoul(optarg,NULL,10);
// 	    if( dev.ttcext != 0 && dev.ttcext != 1 ) { 
// 	      fprintf(stderr,"Error: valid values for ttcext are 0 (local) or 1 (external)\n");
// 	      return 1;
// 	    }
// 	  }
// 	  break;  
	case 'r':  
	  printf("reset: %s\n", optarg);  
	  if( optarg )
	    dev.reset = strtoul(optarg,NULL,10);
	  else dev.reset = MGT_RESET_TOGGLE;
	  break;  
	case 'a':  
	  printf("align: %s\n", optarg);  
	  if( optarg )
	    dev.total_delta = strtol(optarg,NULL,10);
	  dev.mode = MGT_MODE_ALIGN;
	  break;  
	case 'p':  
	  printf("play: %s\n", optarg);  
	  dev.mode = MGT_MODE_PLAY;
	  if(optarg!=NULL) 
	    dev.prefix = strtoul(optarg,NULL,16)<<12UL;
	  else dev.prefix = 0x0;
	  break;  
	case 'v':  
	  printf("dump\n");  
	  dev.dump = 1;
	  break;  
	case 'h':  
	  mgt_arg_help();
	  dev.mode = MGT_MODE_HELP;
	  return 1;  
	case ':':  
	  printf("option needs a value, optarg is %s\n",optarg);  
	  break;  
	case '?':  
	  printf("unknown option: %c\n", optopt); 
	break;  
        }  
    }  

  if(!strlen(dev.device)) { 
    fprintf(stderr,"Error: device must be specified with -d<device> or --device(=)<device>\n");
    return 1;
  }

  if( dev.quad_id < 0 ) {
    fprintf(stderr,"Error: quad must be specified with -q<quad> or --quad(=)<quad>\n");
    return 1;
  }

  if( dev.mode > MGT_MODE_SET && dev.channel > 3 ) {
    fprintf(stderr,"Valid values for --channel in play/capture modes are 0-3\n");  
    return 1;
  }

  if( dev.mode == MGT_MODE_SET && (dev.channel <= 0 || dev.channel > 15 )) {
    fprintf(stderr,"Valid values for --channel in set play/capture modes are 0x1-0xf\n");  
    return 1;
  }


  return 0;
};

