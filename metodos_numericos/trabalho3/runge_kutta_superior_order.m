pkg load symbolic;

function [xi, yi] = runge_kutta_superior_order(f,x0_y0,h_xn)
  h = h_xn(1);
  xn = h_xn(2);
  x0 = x0_y0(1);
  y0 = x0_y0(2);

  x = x0;
  y = y0;

  yi = [];
  xi = [];
  while(x <= xn)
    xi = vertcat(xi,[x]);
    yi = vertcat(yi,[y]);

    k1 = function_handle(f)(x,y);
    k2 = function_handle(f)(x + h/4,y + k1*h/4);
    k3 = function_handle(f)(x + h/4,y + k1*h/8 + k2*h/8);
    k4 = function_handle(f)(x + h/2,y - 1/2*k2*h + k3*h);
    k5 = function_handle(f)(x + 3/4*h, y + 3/16*k1*h + 9/16*k4*h);
    k6 = function_handle(f)(x + h, y - 3/7*k1*h + 2/7*k2*h + 12/7*k3*h - 12/7*k4*h + 8/8*k5*h);

    y_new = y + 1/90*(7*k1 + 32*k3 + 12*k4 + 32*k5 + 7*k6)*h;
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

f = get_f;
x0_y0 = get_initial_values;
h_xn = get_step_xn;
[xi, yi] = runge_kutta_superior_order(f,x0_y0,h_xn);
disp("\nOs valores são: "),disp(horzcat(xi, yi));
