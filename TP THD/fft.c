#include <math.h>
#include <stdint.h>

//void dft(float *xr, float *xi, float *XR, float *XI, int len)
//{
//	int i,j;
//	for(i=0;i<len;i++)
//	{
//		XR[i] = 0.0;
//		XI[i] = 0.0;
//		for(j=0;j<len;j++)
//		{
//			XR[i]+=xr[j]*cos(2*M_PI*i*j/len) +xi[j]*sin(2*M_PI*i*j/len);
//			XI[i]+=xi[j]*cos(2*M_PI*i*j/len) -xr[j]*sin(2*M_PI*i*j/len);
//		}
//	}
//}

/*
 * Invierte los bits de un n�mero dado de len bits.
 * Por ejemplo para 4 bits el 0011 (3) lo invierte
 * como 1100 (12).
 */
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
	uint32_t Nbits = (int)floor(log2(len));

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
