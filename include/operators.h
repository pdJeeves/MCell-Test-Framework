#ifndef _operators_h_
#define _operators_h_


enum __op
{
	__ne__ = 0,
	__eq__ = 1,
	__lt__ = 1 << 1,
	__le__ = __lt__ | __eq__,
	__gt__ = 1 << 2,
	__ge__ = __gt__ | __eq__
};


inline
int op_swap(int i)
{
	if(i & (__lt__ | __gt__))
	{
		return (__op) (i ^ (__lt__ | __gt__));
	}

	return i;
}

inline
int op_reverse(int i)
{
	if(i & (__lt__ | __gt__))
	{
		return (__op) (i ^ (__eq__ | __lt__ | __gt__));
	}

	return (__op) (i ^ __eq__);
}


#endif
