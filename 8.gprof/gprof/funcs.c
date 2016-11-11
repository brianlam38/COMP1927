#include <stdio.h>
void f(), g(), h(), k(), m();

int main(void)
{
	int i, j;

	f();
	for (i = 0; i < 100; ++i) {
		g();
		for (j = 0; j < 200; ++j)
			h();
	}
	f();
}

void f()
{
	int i, j=2;

	for (i = 0; i < 50000; ++i) {
		if (i%3 == 1) k();
		j = (int)((float)j / 1.0);
	}
}

void g()
{
	int i, j=3;

	k(); m();
	for (i = 0; i < 5000; ++i)
		j = (int)((float)j / 1.0);
}

void h()
{
	int i, j=4;

	m();
	for (i = 0; i < 250; ++i)
		j = (int)((float)j / 1.0);
}

void k()
{
	int i, j=5;

	m();
	for (i = 0; i < 50; ++i)
		j = (int)((float)j / 1.0);
}

void m()
{
	int i, j=6;

	for (i = 0; i < 10; ++i)
		j = (int)((float)j / 1.0);
}
