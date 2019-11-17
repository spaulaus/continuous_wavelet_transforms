#include "lorentztools.h"
#include <stdio.h>

// Lorentzian Function :
float lorentz( float x, float g, float x0 ) 
{
  float val;
  val = g/(2.*pi) * 1./ ( pow( g/2., 2.) + pow( x-x0 , 2. ) );
  return val;
} 

// Lorentzian derivative w.r.t 'x' :
float lorentzDx( float x, float g, float x0 ) 
{
  float val;
  val = -g/pi * ( x - x0 ) / pow( pow( g/2., 2.) + pow( x-x0 , 2. ) , 2.) ;
  return val;
}  

// Lorentzian second derivative w.r.t 'x' :
float lorentzD2x( float x, float g, float x0 ) 
{
  float Lval, val;
  Lval = lorentz( x, g, x0 );
  val = 4.*pi/g * ( 8.*pi*pow( x-x0 ,2.) * Lval/g - 1. ) * pow( Lval, 2. ) ;
  return val;
}  

// Lorentzian derivative w.r.t 'g' :
float lorentzDg( float x, float g, float x0 ) 
{
  float Lval, val;
  Lval = lorentz( x, g, x0 );
  val = Lval/g - pi * pow( Lval , 2. ) ;
  return val;
}


// Lorentzian derivative w.r.t 'x0' ( := -lorentDx ):
float lorentzDx0( float x, float g, float x0 ) 
{
  float val;
  val = g/pi * ( x - x0 ) / pow( pow( g/2., 2.) + pow( x-x0 , 2. ) , 2.) ;
  return val;
}


/**********************************************/
// Absoption values :
//
// Returns the coefficent of absorption of a Lorentzian magnitude :
float lorentz_Abs( float x, float g, float x0 )
{
  float val;
  val = pow( 10., -0.4 * lorentz( x, g, x0 ) );
  return val;
}

// Returns the coefficent of absorption of 'lorentzDg' :
float lorentzDg_Abs( float x, float g, float x0 )
{
  float val;
  val = pow( 10., -0.4 * lorentzDg( x, g, x0 ) );
  return val;
}

// Returns the coefficent of absorption of 'lorentzDx0' :
float lorentzDx0_Abs( float x, float g, float x0 )
{
  float val;
  val = pow( 10., -0.4 * lorentzDx0( x, g, x0 ) );
  return val;
}


/*******************************************************/
// Lorentzian Arrays :
//
float* VectLorentz(float *Xvect, float Gamma, float X0, int Sz )
{
  int iC;
  float *Lvect;

  Lvect = new( float[Sz] );

  for(iC=0; iC<Sz; iC++) {
    Lvect[iC] = lorentz( Xvect[iC], Gamma, X0 );
  }

  return Lvect;
}
  
/**********************************************************/
// Lorentzian 2nd derivative Array, Normalized for wavelet
// usage ( sensitive to dGamma ) :
//
float* LorentzDg4wave(float *Xvect, float Gamma, float X0, int Sz )
{
  int iC;
  float *Lvect, Lsum;
  int debug = 1;

  Lvect = new( float[Sz] );

  for(iC=0; iC<Sz; iC++) {
    Lvect[iC] = lorentzDg( Xvect[iC], Gamma, X0 );
  }
  
  // Normalize sum to 0. :
  Lsum = 0.;
  for(iC=0; iC<Sz; iC++) Lsum += Lvect[iC];
  for(iC=0; iC<Sz; iC++) Lvect[iC] -= Lsum/float(Sz);
  
  // Normalize lorentz gamma derivative AutoCorrelation to 1. :
  Lsum = 0.;
  for(iC=0; iC<Sz; iC++) Lsum += Lvect[iC]*Lvect[iC];
  if( Lsum != 0. ) {
    for(iC=0; iC<Sz; iC++) Lvect[iC] /= Lsum;
  }
  else if(debug) printf(" -!nullLorentz!- ");

  return Lvect;
}
  

/*******************************************************/
// Lorentzian Wavelet at 'gamma' FWHM and 'Ng'*'gamma' half Width :
// ! 'Yfilter' must be allocated beforehand at [sz] !
//
int lorentz_filter( float* Xarray, float* Yarray, int sz, float gamma, float Ng, float*& Yfilter )
{
  int k,iW, I1,I2,Lsz;

  float x, Lsum;
  float *lorwave;

  // Loop on array :
  I1=0; I2=0; // Wavelet bounds index.
  for( k=0; k<sz; k++) {
    
    x = Xarray[k];
    while( I1<sz-1 && Xarray[I1] < x - Ng*gamma ) I1 += 1;
    while( I2<sz-1 && Xarray[I2] < x + Ng*gamma ) I2 += 1;

    Lsz = I2 - I1 + 1;
    lorwave = new( float[Lsz] );
    for( iW=0; iW<Lsz; iW++) lorwave[iW] = lorentzD2x( Xarray[I1+iW], gamma, x );

    // Normalize it's sum to 0. :
    //
    Lsum = 0.;
    for( iW=0; iW<Lsz; iW++) {
      Lsum += lorwave[iW] ;
    }
    for( iW=0; iW<Lsz; iW++) lorwave[iW] -= Lsum/float(Lsz);

    // Normalize it's convolution with a lorentzian to 1. :
    //
    Lsum = 0.;
    for( iW=0; iW<Lsz; iW++) {
      Lsum += lorwave[iW] * lorentz( Xarray[I1+iW], gamma, x );
    }
    for( iW=0; iW<Lsz; iW++) lorwave[iW] /= Lsum;

    // Convolve :
    Yfilter[k] = 0.;
    for( iW=0; iW<Lsz; iW++) {
      Yfilter[k] += Yarray[I1+iW] * lorwave[iW];
    }    
    delete[] lorwave;
  }
  
  return 0;
}
  

/*******************************************************/
// Iterative Adaptation of FWHM and Depth of a lorenzian.
// ! 'Yfilter' must be allocated beforehand at [sz] !
//
int lorentz_shape( float* Xs, float* Ys, int sz, float x0, float g0, float& Xout,float& Gout, float& Aout, float Ng, float eps , int debug )
{

  int k, iW, Niter, I1,I2, Imin, Lsz;
  float Lsum, Csum;
  float *vectlet, *Lresidu, *lorDG;

  float v0,v1,v2, dw0,dw2, dy0,dy2, Dv,DDv, min, dG,frac;

  float xpos, ampl, gamma, err; // To return.

  //int debug = 0;
  // Init residuals and variables :
  //
  vectlet  = new( float[sz] );
  Lresidu = new( float[sz] );
  xpos = x0; gamma = g0; err= 1.;

  //for( iW=0; iW<Nlines; iW++) {
  Niter = 0;
  //
  if(debug) printf(" Lorentz Fit from xw=[%.2f|%.2f] : ", x0, g0);
  do {
    //
    // START ITERATIONS on Width and Amplitude of Lorentzian Shape :
    //
    // Apply Wavelet to Data :
    lorentz_filter( Xs,Ys,sz, gamma,Ng, vectlet );
    
    I1 = 0; I2 = 0;
    while( I1<sz-1 && Xs[I1] < x0 - Ng*g0 ) I1 += 1;
    while( I2<sz-1 && Xs[I2] < x0 + Ng*g0 ) I2 += 1;

    Imin = I1; min = vectlet[I1];
    for( k=I1; k<=I2; k++) {
      if( vectlet[k] < min ) { Imin = k; min = vectlet[k];}
    }
    // Check that we don't reach the border :
    if( Imin <= I1 || Imin >= I2 ) {
      printf("  <-  ! Reach Border (W[%d]=%.1f at i=%d) . Dont' estimate. Keep Priors & No Abs. STOP.\n",Imin,Xs[Imin],Niter);
      Xout = x0;
      Gout = g0;
      Aout = 0.;
      return 1;
    }
    
    // Parabolic min :
    v0 = vectlet[Imin-1]; v1 = vectlet[Imin]; v2 = vectlet[Imin+1];
    dw0 = Xs[Imin-1] - Xs[Imin]; dw2 = Xs[Imin+1] - Xs[Imin]; 
    dy0 = v0 - v1; dy2 = v2 - v1;

    Dv = dy0/dw0 - (dy0*dw2 - dy2*dw0 )/( dw2*(dw0 - dw2 ));
    DDv= 2.*( dy0*dw2/dw0 - dy2 )/( dw2*(dw0-dw2));

    xpos = Xs[Imin] - Dv/DDv;
    ampl = v1 - Dv*Dv/( 2.*DDv );

    if( fabs( ampl ) < 1.e-3 ) {
      printf("  <-  ! Min Abs[%d: %.1f Ang]%.1e <1.e-3 . Dont' estimate. Keep Priors & No Abs. STOP.\n",Imin,Xs[Imin],ampl);
      Xout = x0;
      Gout = g0;
      Aout = 0.;
      return 1;
    }
      
    if(debug) printf("#%d#| X%+.1e ; ",Niter, xpos-x0);

    // Substract Lorentzians found :
    for( k=0; k<sz; k++) {
      
      Lresidu[k] = Ys[k];
      Lresidu[k] -= ampl * lorentz( Xs[k], gamma, xpos);
      
    }
    // Iterate lorentzian filter around 'xpos' prior :

    // Convolve with the derivative of the lorentzian w.r.t. 'gamma' :
    //
    I1 = 0; I2 = 0;
    while( I1<sz-1 && Xs[I1] < xpos - Ng*gamma ) I1 += 1;
    while( I2<sz-1 && Xs[I2] < xpos + Ng*gamma ) I2 += 1;
      
    Lsz = I2-I1+1;
    lorDG = LorentzDg4wave( Xs+I1, gamma, xpos, Lsz );

    // Perform convolution at 'xpos' :
    Csum = 0.;
    for(k=I1; k<=I2; k++) {
      Csum += lorDG[k-I1]*Lresidu[k];
    }
    if(debug) printf(" W%+.2e |",-1.*Csum/ampl );

    dG = Csum/ampl;
    frac = fabs( dG/gamma );
    gamma -= dG;

    delete[] lorDG;
    Niter += 1;

    // Abort if Positive Amplitude Found :
    if( ampl > 0. ) {
      printf("  <-  ! Positive Amplitude at i=%d. Keep Priors & No Abs. STOP.\n ",Niter);
      Xout = x0;
      Gout = g0;
      Aout = 0.;
      return 1;
    }
    // Abort if Negative FWHM Found :
    if( gamma < g0 && gamma < fabs(dG) ) {
      printf("-! Too Small FWHM at i=%d. Try to INVERT convergence (%+.1e)|-",Niter, dG);
      // Xout = x0;      Gout = g0;      Aout = -10.;
      //return 1;

      // Try to add intead !? :
      gamma += 2.*dG;
    }
      

  } while( frac > eps &&  Niter < 20 );

  if( Niter == 20 && frac > eps ) {
    printf("   ! STOP at i=%d : EPS=%.2e >%.1e !",Niter,frac,eps);
    printf(" KEEP Pos = %.1f(%+.1e) Ang. FWHM = %.1f Ang. I=%.1e.\n",xpos,x0-xpos, gamma,ampl);
  }

  Xout = xpos;
  Gout = gamma;
  Aout = ampl;

  printf("  <-  LorentzShape from xw[%.1f|%.1f] -> xiw[%.1f|%.1f|%.1f] in %d.\n",x0,g0,Xout,Aout,Gout, Niter);

  delete[] Lresidu; delete[] vectlet;

  return 0;
}
  




