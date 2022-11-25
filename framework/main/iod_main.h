// IOD内部用
#define GPIO5_PORT      (5)
#define GPIO18_PORT     (18)
#define GPIO19_PORT     (19)
#define GPIO21_PORT     (21)
#define GPIO22_PORT     (22)
#define GPIO23_PORT     (23)

// iod_main
extern void iod_init(void);
extern void iod_deinit(void);
extern void iod_reinit(void);
extern void iod_main_1ms(void);
extern void iod_main_in(void);
extern void iod_main_out(void);
// iod_port
extern void iod_port_init(void);
extern void iod_port_deinit(void);
extern void iod_port_reinit(void);
extern void iod_port_main_1ms(void);
extern void iod_port_main_in(void);
extern void iod_port_main_out(void);
extern void iod_read_btn0(bool *);
extern void iod_read_btn1(bool *);
extern void iod_write_led0(bool);
extern void iod_write_led1(bool);
extern void iod_write_port0(bool);
extern void iod_write_port1(bool);
