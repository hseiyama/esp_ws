typedef struct {
    uint64_t u64_time;
    bool bl_state;
} ST_SYS_TIMER;

extern void sys_call_timer_start(ST_SYS_TIMER *);
extern void sys_call_timer_stop(ST_SYS_TIMER *);
extern bool sys_call_timer_check(ST_SYS_TIMER *, uint64_t);
extern uint64_t sys_call_timer_diff(ST_SYS_TIMER *, uint64_t);
extern bool sys_call_timer_isrun(ST_SYS_TIMER *);
