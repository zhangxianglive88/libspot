#include "streammoments.h"

/* VERY LIGHT VERSION (only mean)*/
StreamMean::StreamMean(int size) : Ubend(size)
{
	this->m = 0.0;
}

StreamMean::StreamMean(int size, vector<double> v) : 
StreamMean(size)
{
	for(auto x : v)
	{
		this->StreamMean::step(x);
	}
}


StreamMean::StreamMean(const Ubend & other) : Ubend(other)
{
	this->m = 0.0;
}


int StreamMean::step(double x_n)
{
	int status = this->push(x_n);
	int n = this->size();
	
	double M = this->m;
	double delta, delta_n;
	
	if (status <= 0)
	{
		delta = x_n - M;
		delta_n = delta/n;
		this->m = M + delta_n;
	}
	else
	{
		double x_1 = this->last_erased_data;
		delta = x_n - x_1;
		delta_n = delta/n;
		this->m = this->m + delta_n;
	}
	
	
	return status;
}


double StreamMean::mean()
{
	return(this->m);
}



StreamMean StreamMean::operator+(const StreamMean& other) const
{
	StreamMean sum(this->Ubend::merge(other));
	sum.m = std::accumulate(sum.begin(), sum.end(), 0.0) / sum.size();
	return sum;
}




/* LIGHT VERSION (only mean and variance)*/

StreamVar::StreamVar(int size) : StreamMean(size)
{
	this->m2 = 0.0;
}


StreamVar::StreamVar(int size, vector<double> v) : StreamVar(size)
{
	for(auto x : v)
	{
		this->StreamVar::step(x);
	}
}


double StreamVar::var()
{
	return this->m2/this->size();
}


int StreamVar::step(double x_n)
{
	int status = this->push(x_n);
	int n = this->size();
	
	double M = this->m;
	double delta, delta_n;
	
	if (status <= 0)
	{
		delta = x_n - M;
		delta_n = delta/n;
	
		this->m = M + delta_n;
		this->m2 = this->m2 + (n-1.) * delta * delta_n;
	}
	else
	{
		double x_1 = this->last_erased_data;
		delta = x_n - x_1;
		delta_n = delta/n;
		
		this->m = this->m + delta_n;
		double u = x_n - this->m;
		this->m2 = this->m2 + delta * ( u + x_1 - M );
	}
	
	return status;
}





/* FULL VERSION (mean, variance and M3)*/

StreamMoments::StreamMoments(int size) : StreamVar(size)
{
	this->m3 = 0.0;
}


StreamMoments::StreamMoments(int size, vector<double> & v) : StreamMoments(size)
{
	for(auto & x: v)
	{
		this->StreamMoments::step(x);
	}
}

/*
template<typename DATA>
StreamMoments<DATA>::StreamMoments(int size, DATA & data) : StreamVar(size)
{
	for(auto & x: data)
	{
		this->StreamMoments::step(x);
	}
}*/


double StreamMoments::M3()
{
	return this->m3/this->size();
}


int StreamMoments::step(double x_n)
{
	int status = this->push(x_n);
	int n = this->size();
	
	double M = this->m;
	double delta, delta_n, delta_n2;
	
	if (status <= 0)
	{
		delta = x_n - M;
		delta_n = delta/n;
		delta_n2 = delta_n * delta_n;
	
		this->m = M + delta_n;
		this->m3 = this->m3 + (n-1.)*(n-2.)*delta*delta_n2 - 3. * delta_n * (this->m2) ;
		this->m2 = this->m2 + (n-1.) * delta * delta_n;
	}
	else
	{
		double x_1 = this->last_erased_data;
		delta = x_n - x_1;
		delta_n = delta/n;
		delta_n2 = delta_n * delta_n;
		
		this->m = this->m + delta_n;
		double u = x_n - this->m;
		double v = x_1 - this->m;
		

		this->m3 = this->m3 - delta_n * ( 3. * this->m2 - n * delta_n2 ) + delta * ( pow(delta,2)+3*u*v ); 
		this->m2 = this->m2 + delta * ( u + x_1 - M );
	}
	
	
	return status;
}



/*
StreamMoments::StreamMoments(int size) : Ubend(size)
{
	this->m = 0.0;
	this->M2 = 0.0;
	this->M3 = 0.0;
}


StreamMoments::StreamMoments(int size, vector<double> v) : StreamMoments(size)
{
	for(auto x : v)
	{
		this->step(x);
	}
}


double StreamMoments::getMoment(int k)
{
	switch (k)
	{
		case 1:
			return this->m;
			break;
		case 2:
			return this->M2/this->size();
		case 3:
			return this->M3/this->size();
		default:
			cout << "Unknown moment" << endl;
			return(0.0);
	}
	
}

int StreamMoments::step(double x_n)
{
	int status = this->push(x_n);
	int n = this->size();
	
	double M = this->m;
	double delta, delta_n, delta_n2;
	
	if (status <= 0)
	{
		delta = x_n - M;
		delta_n = delta/n;
		delta_n2 = delta_n * delta_n;
	
		this->m = M + delta_n;
		this->M3 = this->M3 + (n-1.)*(n-2.)*delta*delta_n2 - 3. * delta_n * (this->M2) ;
		this->M2 = this->M2 + (n-1.) * delta * delta_n;
		//cout << this->M2 << endl;
	}
	else
	{
		double x_1 = this->last_erased_data;
		delta = x_n - x_1;
		delta_n = delta/n;
		delta_n2 = delta_n * delta_n;
		
		this->m = this->m + delta_n;
		double u = x_n - this->m;
		double v = x_1 - this->m;
		

		this->M3 = this->M3 - delta_n * ( 3. * this->M2 - n * delta_n2 ) + delta * ( pow(delta,2)+3*u*v ); 
		this->M2 = this->M2 + delta * ( u + x_1 - M );
	}
	
	
	return status;
}
*/

