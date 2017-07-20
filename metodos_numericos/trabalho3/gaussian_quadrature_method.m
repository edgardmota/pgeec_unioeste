pkg load symbolic

function integral = gaussian_quadrature_method(f,ab,p,w)
  syms t x;

  b = ab(2);
  a = ab(1);

  g = subs(f,x,1/2*(a+b + t*(b-a)));
  dt = (b-a)/2;

  s = get_size(p);
  integral = 0;
  for i = 1:(s+1)
    integral += w(i)*function_handle(g)(p(i));
  end
  integral *= dt;
end

function [x,w]=points_and_weights(N)
  N=N-1;
  N1=N+1; N2=N+2;

  xu=linspace(-1,1,N1)';

  y=-1*(cos((2*(0:N)'+1)*pi/(2*N+2))+(0.27/N1)*sin(pi*xu*N/N2));

  L=zeros(N1,N2);
  Lp=zeros(N1,N2);

  y0=2;

  while max(abs(y-y0))>eps
      L(:,1)=1;
      Lp(:,1)=0;

      L(:,2)=y;
      Lp(:,2)=1;

      for k=2:N1
          L(:,k+1)=( (2*k-1)*y.*L(:,k)-(k-1)*L(:,k-1) )/k;
      end

      Lp=(N2)*( L(:,N1)-y.*L(:,N2) )./(1-y.^2);

      y0=y;
      y=y0-L(:,N2)./Lp;
  end
  x=y;

  w=2./((1-y.^2).*Lp.^2)*(N2/N1)^2;
end

function s = get_size(X)
  s = size(X,2);
end

function N = get_degree
  N = input("\nEntre com o grau desejado: ");
end

function f = get_f
  syms x;
  f = input('Entre com a funçao f(x): ');
end

function ab = get_ab
  ab = zeros(1,2);
  disp("\nIntervalo de [a,b]:\n");
  ab(1) = input('Entre com o valor de a: ');
  ab(2) = input('Entre com o valor de b: ');
end

f = get_f;
ab = get_ab;
N = get_degree;
[p, w] = points_and_weights(N);
p
w
integral = gaussian_quadrature_method(f,ab,p,w);
disp("\nO valor da integral é: "),disp(integral);
