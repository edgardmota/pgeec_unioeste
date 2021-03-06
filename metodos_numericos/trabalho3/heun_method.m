pkg load symbolic;

function [xi, yi] = heun_method(f,x0_y0,h_xn,i)
  h = h_xn(1);
  xn = h_xn(2);
  x0 = x0_y0(1);
  y0 = x0_y0(2);

  x = x0;
  y = y0;
  y_new = y0;
  yi = [];
  xi = [];
  while(x <= xn)
    n = i;
    xi = vertcat(xi,[x]);
    yi = vertcat(yi,[y]);

    y_new = y + function_handle(f)(x,y)*h;
    while(n)
      y_new = y + (function_handle(f)(x,y) + function_handle(f)(x+h,y_new))/2*h;
      n -= 1;
    end
    x += h;
    y = y_new;
  end
end

function f = get_f
  syms x y;
  f = input("\nEntre com a funçao f(x,y): ");
end

function h_xn = get_step_xn
  h_xn = zeros(1,2);
  h_xn(1) = input("\nEntre com passo (h): ");
  h_xn(2) = input('Entre com o valor de xn: ');
end

function x0_y0 = get_initial_values
  x0_y0 = zeros(1,2);
  x0_y0(1) = input("\nEntre com o valor de x0: ");
  x0_y0(2) = input('Entre com o valor de y0: ');
end

function i = get_interations
  i = input("\nEntre com o nº de interações: ");
end


f = get_f;
x0_y0 = get_initial_values;
h_xn = get_step_xn;
i = get_interations;
[xi, yi] = heun_method(f,x0_y0,h_xn,i);
disp("\nOs valores são: "),disp(horzcat(xi, yi));
