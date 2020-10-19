#include <math.h>
#include <stdint.h>

extern const unsigned int FFTConst;

static uint32_t bit_reversal(uint32_t bi, int len)
{
	int i;
	uint32_t bo = 0;
	for(i=0;i<len;i++)
	{
		bo <<= 1;
		bo |= (bi & 1);
		bi >>= 1;
	}
	return bo;
}

static uint32_t nbits(uint32_t n)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (n & (1 << 31))
			break;
		n <<= 1;
	}
	return 32 - i - 1;
}


/*
 * FFT Cooley-Tukey decimada en tiempo.
 * Recibe dos vectores de datos (parte real e imaginaria)
 * y pone la transformada de fourier en esos mismos vectores.
 * la �nica restricci�n para esto es que len sea potencia de
 * dos.
 *
 *
 */
void fft_radix2(float *xr, float *xi, int len)
{
	int i,j;
	float t_r, t_i;
	uint32_t Nbits = nbits(len);

	for(i=0;i<len;i++)
	{
		j = bit_reversal(i, Nbits);
		if(j>i)
		{
			t_r = xr[i];
			t_i = xi[i];
			xr[i] = xr[j];
			xi[i] = xi[j];
			xr[j] = t_r;
			xi[j] = t_i;
		}
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= len; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = len / size;
		for (size_t i = 0; i < len; i += size)
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{
					size_t l = j + halfsize;
					t_r=xr[l]*cos(2*3.1415*k/len) +xi[l]*sin(2*3.1415*k/len);
					t_i=xi[l]*cos(2*3.1415*k/len) -xr[l]*sin(2*3.1415*k/len);
					xr[l] = xr[j]-t_r;
					xi[l] = xi[j]-t_i;
					xr[j]+= t_r;
					xi[j]+= t_i;
			}
		}
		if (size == len)
			break;
	}
}
