
#ifndef MATH_FUNCTION_FUNCTION

#define MATH_FUNCTION_FUNCTION


struct data_q_struct {
    long data;
    char q;
};

//计算单精度浮点数x和y的和并将结果返回
extern float add_float(float x, float y);

//计算单精度浮点数x和y的差并将结果返回:y-x
extern float sub_float(float x, float y);

//单精度浮点数转化为长整型并将结果返回
extern long  float_to_int(float x);

//长整型转化为单精度浮点数并将结果返回
extern float int_to_float(long x);

//长整型x 的余弦（cos）的固定点表示并将结果返回
extern long  cos_fix(long x);

//单精度浮点数x的余弦（cos）的固定点表示并将结果返回
extern float cos_float(float x);

//长整型x 的正弦（cos）的固定点表示并将结果返回
extern long  sin_fix(long x);

//单精度浮点数x的正弦（sin）的固定点表示并将结果返回
extern float sin_float(float x);

extern struct data_q_struct complex_abs_fix(long x, long y);

extern struct data_q_struct complex_dqdt_fix(long x, long y);

//计算复数的模长并将结果返回:(x^2 + y^2)^0.5
extern float  complex_abs_float(float x, float y);

//计算复数的差商并将结果返回:(x^2 - y^2)^0.5
extern float  complex_dqdt_float(float x, float y);

extern struct data_q_struct root_fix(struct data_q_struct x);

//计算浮点数x的平方根值,并返回计算结果
extern float  root_float(float x);

extern struct data_q_struct mul_fix(long x, long y);

//计算单精度浮点数变量x和y的乘法运算，并将计算结果返回
extern float  mul_float(float x, float y);

extern struct data_q_struct div_fix(long x, long y);

//计算单精度浮点数变量x和y的除法运算，并将计算结果返回:y/x
extern float  div_float(float x, float y);

extern struct data_q_struct exp_fix(long x);

// ; e^x，也就是e的x次方，并将计算结果返回
extern float  exp_float(float x);

extern struct data_q_struct ln_fix(struct data_q_struct x);

//计算x的自然对数：ln(x)，并将计算结果返回
extern float  ln_float(float x);

extern struct data_q_struct angle_fix(long x, long y);

//计算给定两个浮点数 x 和 y 的反正切角度（arctan）的弧度值并将结果返回
extern float angle_float(float x, float y);

extern struct data_q_struct atanh_fix(long x, long y);

//计算给定两个浮点数 x 和 y 的反双曲正切（atanh）的值,并将计算结果返回
extern float atanh_float(float x, float y);

//计算给长整型x 的双曲余弦（cosh）的固定点表示并将结果返回
extern long  cosh_fix(long x);

//计算给定浮点数 x 的双曲余弦（cosh）的值并将结果返回
extern float cosh_float(float x);

//计算给定整数 x 的双曲正弦（sinh）的固定点表示
extern long  sinh_fix(long x);

//计算给定浮点数 x 的双曲正弦（sinh）的值并将结果返回
extern float sinh_float(float x);

extern struct data_q_struct log10_fix(struct data_q_struct x);

//计算给定浮点数x的以10为底的对数,并将计算结果返回
extern float  log10_float(float x);

extern struct data_q_struct sigmoid_fix(float x);

//计算给定浮点数的x的sigmoid函数值并将结果返回
extern float  sigmoid_float(float x);

extern struct data_q_struct tanh_fix(float x);

//计算给定浮点数x的双曲正切函数值，并将结果返回
extern float  tanh_float(float x);


#endif

