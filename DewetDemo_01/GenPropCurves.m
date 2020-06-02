%Quick script to generate material property interpolations for metal going
%through phase change

T_min = 100; %K
T_max = 1000; %K
T_PC = 500; %K
PC_T_Span = 10; %K
N_Span = 100;

cp_S = 500; %J/kg-K
cp_L = 600; %J/kg-K
h_SL = 100E3; %J/kg-K

rho_L = 5000;
rho_S = 5000;

mu_L = 0.005; %kg/m-s
mu_S = 1000*mu_L; %kg/m-s

k_S = 30; %W/m-K
k_L = 10; %W/m-K

Ts = [ T_min ...
       linspace(T_PC - PC_T_Span/2, T_PC + PC_T_Span/2, N_Span) ...
       T_max ];

mus = [ mu_S ...
        mu_S + (mu_L-mu_S)*sin( ( Ts(2:end-1) - Ts(2) )*(pi/2)/PC_T_Span ).^2  ...
        mu_L ];

ks  = [ k_S ...
        k_S + (k_L-k_S)*sin( ( Ts(2:end-1) - Ts(2) )*(pi/2)/PC_T_Span ).^2  ...
        k_L ];
    
rhos = [ rho_S ...
         rho_S + (rho_L-rho_S)*sin( ( Ts(2:end-1) - Ts(2) )*(pi/2)/PC_T_Span ).^2  ...
         rho_L ];
    
cps  = [ cp_S ...
         cp_S + (cp_L-cp_S)*sin( ( Ts(2:end-1) - Ts(2) )*(pi/2)/PC_T_Span ).^2  ...
         + h_SL*sin( ( Ts(2:end-1) - Ts(2) )*pi/PC_T_Span ).^2 * (2/PC_T_Span) ...
         cp_L ];
hs = [0 cumsum( cps(2:end).*( Ts(2:end) - Ts(1:end-1) ) ) ];

alphas = rhos.*cps./ks;

%Write out to text
WriteOut(Ts, cps, 'Ts_cps.dat');
WriteOut(Ts, rhos, 'Ts_rhos.dat');
WriteOut(Ts, mus, 'Ts_mus.dat');
WriteOut(Ts, ks, 'Ts_ks.dat');
WriteOut(Ts, alphas, 'Ts_alphas.dat');
WriteOut(Ts, hs, 'Ts_hs.dat');
WriteOut(hs, Ts, 'hs_Ts.dat');

function WriteOut(A, B, Name)
    FID = fopen(strcat('constant', filesep, 'metalProperties', filesep, Name),'w');
    fprintf(FID, '(\n');
    for j = 1:length(A)
       fprintf(FID, '\t(%5e %5e)\n', A(j), B(j));
    end
    fprintf(FID, ');\n');
    fclose(FID);
end
    
    