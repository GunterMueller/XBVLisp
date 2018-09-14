
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 	 actions.h
 * 	Ernesto Sendoya	
 *      janvier 1992
 */


#ifdef STDC_HEADERS

/* xactions.c <<<<<<<<<<<< */
extern void delete_next_characterXbvl(XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void delete_next_atomeXbvl(XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void delete_prev_atomeXbvl(XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void delete_previous_characterXbvl( XbvlispWidget w, XKeyEvent * event , String * params, Cardinal * num_params );
extern void shift_car_rightXbvl( XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void kill_to_end_of_lineXbvl(XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void end_of_lineXbvl(XbvlispWidget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void unkill_der_killXbvl(Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void insert_selection(Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void delete_selection(Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void PlaceMenu( Widget w, XButtonEvent * event,  String * params, Cardinal * num_params);
extern void liste_aide(Widget w, XKeyEvent *event, String * params,  Cardinal * num_params);
extern void select_aide( Widget w, XKeyEvent *event, String *params, Cardinal * num_params);
extern void EvalSelection(Widget w, XEvent *event, String *params, Cardinal *num_params);
extern void toggle_insert_mode(Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
#else 
extern void delete_next_characterXbvl();
extern void delete_next_atomeXbvl();
extern void delete_prev_atomeXbvl();
extern void delete_previous_characterXbvl();
extern void shift_car_rightXbvl();
extern void kill_to_end_of_lineXbvl();
extern void end_of_lineXbvl();
extern void unkill_der_killXbvl();
extern void insert_selection();
extern void delete_selection();
extern void end_of_lineXbvl();
extern void EvalSelection();
extern void toggle_insert_mode();
#endif








