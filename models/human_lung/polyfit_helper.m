function p=polyfit_helper(y,nn,T,n)

t=(0:T:n*T)';
y=y(end-n:end);

p=polyfit(t,y,nn);

