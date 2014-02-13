/* user_interface.cc */

void kleinian_group::user_interface(){
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
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Right){	// rotate right
                	CAMERA=build_mat(0,2,0.01)*CAMERA;
                	draw_state();
                	break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Left){	// rotate left
                	CAMERA=build_mat(0,2,-0.01)*CAMERA;
                	draw_state();
                	break;      
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Up){		// rotate up
                	CAMERA=build_mat(1,2,-0.01)*CAMERA;
                	draw_state();
                	break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Down){	// rotate down
                	CAMERA=build_mat(1,2,0.01)*CAMERA;
                	draw_state();
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