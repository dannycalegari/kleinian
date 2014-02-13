/* user_interface.cc */

void kleinian_group::X_user_interface(){
	point p;
	bool finished;

	
	finished=false;
	while(finished==false){
		XNextEvent(display, &report);
		switch(report.type) {
			case ButtonPress:
				p=mouse_location();
				break;
			case KeyPress:
				if(XLookupKeysym(&report.xkey, 0) == XK_q){ // quit           
                    finished=true;
                    XCloseDisplay(display);
                    exit(0);
                    break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_e){	// eps output
                	draw_eps();
                	break;
                };
            default:
            	break;
        };
    };
};