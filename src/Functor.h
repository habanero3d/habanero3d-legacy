/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
This file is part of Habanero3d.

Habanero3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Habanero3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Habanero3d.  If not, see <http://www.gnu.org/licenses/>.


*/


/*! @file
@author qfel

Wesola definicja polimorficznego funktora, i kilka pomocniczych.
Functor<RT, T1, T2, ..., Tn> - funktor zwracajacy RT i bioracy argumenty typu T1, ..., Tn.
::caller(func, a1, a2, ..., an) - tworzy funktor 0-argumentowy, zwracajacy func(a1, ..., an).
UWAGA: zwraca wskazniki, ktora trzeba na koncu potraktowac delete'm
@code
	(*f)();
@endcode
Obsluguje funkcje do 5 argumentow (ale caly naglowek jest generowany skryptem w pythonie, wiec w razie potrzeby...)

Przyklad:
@code
	int mul(int x, int y)
	{
		return x * y;
	}

	Functor<int> &f = *caller(mul, 7, 6);
	printf("%d\n", f());
@endcode
*/
#pragma once

// Typ ktorego sie nie uzywa
struct NOTYPE;

template <typename RT, typename T1 = NOTYPE, typename T2 = NOTYPE, typename T3 = NOTYPE, typename T4 = NOTYPE, typename T5 = NOTYPE>
class Functor
{
public:
	virtual ~Functor() {}
	virtual RT operator()(T1, T2, T3, T4, T5) const = 0;
};

template <typename RT>
class Functor<RT>
{
public:
	virtual ~Functor() {}
	virtual RT operator()() const = 0;
};

template <typename RT, typename T1>
class Functor<RT, T1>
{
public:
	virtual ~Functor() {}
	virtual RT operator()(T1) const = 0;
};

template <typename RT, typename T1, typename T2>
class Functor<RT, T1, T2>
{
public:
	virtual ~Functor() {}
	virtual RT operator()(T1, T2) const = 0;
};

template <typename RT, typename T1, typename T2, typename T3>
class Functor<RT, T1, T2, T3>
{
public:
	virtual ~Functor() {}
	virtual RT operator()(T1, T2, T3) const = 0;
};

template <typename RT, typename T1, typename T2, typename T3, typename T4>
class Functor<RT, T1, T2, T3, T4>
{
public:
	virtual ~Functor() {}
	virtual RT operator()(T1, T2, T3, T4) const = 0;
};

template <typename RT, typename T0 = NOTYPE, typename T1 = NOTYPE, typename T2 = NOTYPE, typename T3 = NOTYPE, typename T4 = NOTYPE>
class FunctionCaller : public Functor<RT>
{
private:
	typedef RT (*FT)(T0, T1, T2, T3, T4);
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
	T3 a3;
	T4 a4;
public:
	FunctionCaller(FT func, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) :
		func(func),
		a0(a0),
		a1(a1),
		a2(a2),
		a3(a3),
		a4(a4)
	{
	}

	virtual RT operator ()() const
	{
		return func(a0, a1, a2, a3, a4);
	}
};

template <typename RT>
class FunctionCaller<RT> : public Functor<RT>
{
private:
	typedef RT (*FT)();
	FT func;
public:
	FunctionCaller(FT func) :
		func(func)
	{
	}

	virtual RT operator ()() const
	{
		return func();
	}
};

template <typename RT, typename T0>
class FunctionCaller<RT, T0> : public Functor<RT>
{
private:
	typedef RT (*FT)(T0);
	FT func;
	T0 a0;
public:
	FunctionCaller(FT func, T0 a0) :
		func(func),
		a0(a0)
	{
	}

	virtual RT operator ()() const
	{
		return func(a0);
	}
};

template <typename RT, typename T0, typename T1>
class FunctionCaller<RT, T0, T1> : public Functor<RT>
{
private:
	typedef RT (*FT)(T0, T1);
	FT func;
	T0 a0;
	T1 a1;
public:
	FunctionCaller(FT func, T0 a0, T1 a1) :
		func(func),
		a0(a0),
		a1(a1)
	{
	}

	virtual RT operator ()() const
	{
		return func(a0, a1);
	}
};

template <typename RT, typename T0, typename T1, typename T2>
class FunctionCaller<RT, T0, T1, T2> : public Functor<RT>
{
private:
	typedef RT (*FT)(T0, T1, T2);
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
public:
	FunctionCaller(FT func, T0 a0, T1 a1, T2 a2) :
		func(func),
		a0(a0),
		a1(a1),
		a2(a2)
	{
	}

	virtual RT operator ()() const
	{
		return func(a0, a1, a2);
	}
};

template <typename RT, typename T0, typename T1, typename T2, typename T3>
class FunctionCaller<RT, T0, T1, T2, T3> : public Functor<RT>
{
private:
	typedef RT (*FT)(T0, T1, T2, T3);
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
	T3 a3;
public:
	FunctionCaller(FT func, T0 a0, T1 a1, T2 a2, T3 a3) :
		func(func),
		a0(a0),
		a1(a1),
		a2(a2),
		a3(a3)
	{
	}

	virtual RT operator ()() const
	{
		return func(a0, a1, a2, a3);
	}
};

template <typename RT>
inline FunctionCaller<RT>* caller(RT (*func)())
{
	return new FunctionCaller<RT>(func);
}

template <typename RT, typename T0>
inline FunctionCaller<RT, T0>* caller(RT (*func)(T0 ), T0 a0)
{
	return new FunctionCaller<RT, T0>(func, a0);
}

template <typename RT, typename T0, typename T1>
inline FunctionCaller<RT, T0, T1>* caller(RT (*func)(T0 , T1 ), T0 a0, T1 a1)
{
	return new FunctionCaller<RT, T0, T1>(func, a0, a1);
}

template <typename RT, typename T0, typename T1, typename T2>
inline FunctionCaller<RT, T0, T1, T2>* caller(RT (*func)(T0 , T1 , T2 ), T0 a0, T1 a1, T2 a2)
{
	return new FunctionCaller<RT, T0, T1, T2>(func, a0, a1, a2);
}

template <typename RT, typename T0, typename T1, typename T2, typename T3>
inline FunctionCaller<RT, T0, T1, T2, T3>* caller(RT (*func)(T0 , T1 , T2 , T3 ), T0 a0, T1 a1, T2 a2, T3 a3)
{
	return new FunctionCaller<RT, T0, T1, T2, T3>(func, a0, a1, a2, a3);
}

template <typename RT, typename T0, typename T1, typename T2, typename T3, typename T4>
inline FunctionCaller<RT, T0, T1, T2, T3, T4>* caller(RT (*func)(T0 , T1 , T2 , T3 , T4 ), T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)
{
	return new FunctionCaller<RT, T0, T1, T2, T3, T4>(func, a0, a1, a2, a3, a4);
}

template <typename T, typename RT, typename T0 = NOTYPE, typename T1 = NOTYPE, typename T2 = NOTYPE, typename T3 = NOTYPE, typename T4 = NOTYPE>
class MethodCaller : public Functor<RT>
{
private:
	typedef RT (T::*FT)(T0, T1, T2, T3, T4);
	T *inst;
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
	T3 a3;
	T4 a4;
public:
	MethodCaller(T *inst, FT func, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) :
		inst(inst),
		func(func),
		a0(a0),
		a1(a1),
		a2(a2),
		a3(a3),
		a4(a4)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)(a0, a1, a2, a3, a4);
	}
};

template <typename T, typename RT>
class MethodCaller<T, RT> : public Functor<RT>
{
private:
	typedef RT (T::*FT)();
	T *inst;
	FT func;
public:
	MethodCaller(T *inst, FT func) :
		inst(inst),
		func(func)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)();
	}
};

template <typename T, typename RT, typename T0>
class MethodCaller<T, RT, T0> : public Functor<RT>
{
private:
	typedef RT (T::*FT)(T0);
	T *inst;
	FT func;
	T0 a0;
public:
	MethodCaller(T *inst, FT func, T0 a0) :
		inst(inst),
		func(func),
		a0(a0)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)(a0);
	}
};

template <typename T, typename RT, typename T0, typename T1>
class MethodCaller<T, RT, T0, T1> : public Functor<RT>
{
private:
	typedef RT (T::*FT)(T0, T1);
	T *inst;
	FT func;
	T0 a0;
	T1 a1;
public:
	MethodCaller(T *inst, FT func, T0 a0, T1 a1) :
		inst(inst),
		func(func),
		a0(a0),
		a1(a1)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)(a0, a1);
	}
};

template <typename T, typename RT, typename T0, typename T1, typename T2>
class MethodCaller<T, RT, T0, T1, T2> : public Functor<RT>
{
private:
	typedef RT (T::*FT)(T0, T1, T2);
	T *inst;
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
public:
	MethodCaller(T *inst, FT func, T0 a0, T1 a1, T2 a2) :
		inst(inst),
		func(func),
		a0(a0),
		a1(a1),
		a2(a2)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)(a0, a1, a2);
	}
};

template <typename T, typename RT, typename T0, typename T1, typename T2, typename T3>
class MethodCaller<T, RT, T0, T1, T2, T3> : public Functor<RT>
{
private:
	typedef RT (T::*FT)(T0, T1, T2, T3);
	T *inst;
	FT func;
	T0 a0;
	T1 a1;
	T2 a2;
	T3 a3;
public:
	MethodCaller(T *inst, FT func, T0 a0, T1 a1, T2 a2, T3 a3) :
		inst(inst),
		func(func),
		a0(a0),
		a1(a1),
		a2(a2),
		a3(a3)
	{
	}

	virtual RT operator ()() const
	{
		return (inst->*func)(a0, a1, a2, a3);
	}
};

template <typename T, typename RT>
inline MethodCaller<T, RT>* caller(T *inst, RT (T::*func)())
{
	return new MethodCaller<T, RT>(inst, func);
}

template <typename T, typename RT, typename T0>
inline MethodCaller<T, RT, T0>* caller(T *inst, RT (T::*func)(T0 ), T0 a0)
{
	return new MethodCaller<T, RT, T0>(inst, func, a0);
}

template <typename T, typename RT, typename T0, typename T1>
inline MethodCaller<T, RT, T0, T1>* caller(T *inst, RT (T::*func)(T0 , T1 ), T0 a0, T1 a1)
{
	return new MethodCaller<T, RT, T0, T1>(inst, func, a0, a1);
}

template <typename T, typename RT, typename T0, typename T1, typename T2>
inline MethodCaller<T, RT, T0, T1, T2>* caller(T *inst, RT (T::*func)(T0 , T1 , T2 ), T0 a0, T1 a1, T2 a2)
{
	return new MethodCaller<T, RT, T0, T1, T2>(inst, func, a0, a1, a2);
}

template <typename T, typename RT, typename T0, typename T1, typename T2, typename T3>
inline MethodCaller<T, RT, T0, T1, T2, T3>* caller(T *inst, RT (T::*func)(T0 , T1 , T2 , T3 ), T0 a0, T1 a1, T2 a2, T3 a3)
{
	return new MethodCaller<T, RT, T0, T1, T2, T3>(inst, func, a0, a1, a2, a3);
}

template <typename T, typename RT, typename T0, typename T1, typename T2, typename T3, typename T4>
inline MethodCaller<T, RT, T0, T1, T2, T3, T4>* caller(T *inst, RT (T::*func)(T0 , T1 , T2 , T3 , T4 ), T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)
{
	return new MethodCaller<T, RT, T0, T1, T2, T3, T4>(inst, func, a0, a1, a2, a3, a4);
}