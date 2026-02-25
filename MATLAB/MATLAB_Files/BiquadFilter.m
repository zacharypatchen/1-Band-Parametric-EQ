% This class creates a BQF with LP, HP, & Non or Constant 
% Parametric capabilities.

% Author: Zachary Patchen
% Version: 4/15/2025

classdef BiquadFilter
    properties
        gain;
        a0;
        a1;
        b1;
        a2;
        b2;
        c0;
        d0;
        theta;
        gamma;
        beta;
        Q;
        d;
        fc;
        fs;
    end

    methods
        % Constructor
        function bqf = BiquadFilter(fc, fs, Q, gain)
            if nargin > 0
                bqf.fc = fc;
                bqf.fs = fs;
                bqf.Q = Q;
                bqf.gain = gain;
            end
        end
        function bqf = filterSelect(bqf, filter_type)
            if strcmp(filter_type, "1oLPF")
                bqf.theta = (2*pi*bqf.fc)/bqf.fs;
                bqf.gamma = (cos(bqf.theta)/(1+sin(bqf.theta)));
                bqf.a0 = (1-bqf.gamma)/2;
                bqf.a1 = (1 - bqf.gamma)/2;
                bqf.a2 = 0;
                bqf.b1 = -(bqf.gamma);
                bqf.b2 = 0;
            elseif strcmp(filter_type,"1oHPF")
                bqf.theta = (2*pi*bqf.fc)/bqf.fs;
                bqf.gamma = (cos(bqf.theta)/(1+sin(bqf.theta)));
                bqf.a0 = (1+bqf.gamma)/2;
                bqf.a1 = -((1 + bqf.gamma)/2);
                bqf.a2 = 0;
                bqf.b1 = -(bqf.gamma);
                bqf.b2 = 0;
            elseif strcmp(filter_type, "2oLPF")
                bqf.theta = (2*pi*bqf.fc)/bqf.fs;
                bqf.d = 1/bqf.Q;
                bqf.beta = (1/2)* ((1-(bqf.d/2)*sin(bqf.theta))/(1 + (bqf.d...
                    /2)*sin(bqf.theta)));
                bqf.gamma = (0.5+bqf.beta)*cos(bqf.theta);
                bqf.a0 = (.5+bqf.beta-bqf.gamma)/2;
                bqf.a1 = 0.5+bqf.beta-bqf.gamma;
                bqf.a2 = (0.5 + bqf.beta - bqf.gamma)/2;
                bqf.b1 = -2*bqf.gamma;
                bqf.b2 = 2*bqf.beta;
            elseif strcmp(filter_type,"2oHPF")
                bqf.theta = (2*pi*bqf.fc/bqf.fs);
                bqf.d = 1/bqf.Q;
                bqf.beta = (1/2)* ((1-(bqf.d/2)*sin(bqf.theta))...
                    /(1 + (bqf.d/2)*sin(bqf.theta)));
                bqf.gamma = (0.5+bqf.beta)*cos(bqf.theta);
                bqf.a0 = (.5+bqf.beta+bqf.gamma)/2;
                bqf.a1 = -(0.5+bqf.beta+bqf.gamma);
                bqf.a2 = (0.5 + bqf.beta + bqf.gamma)/2;
                bqf.b1 = -2*bqf.gamma;
                bqf.b2 = 2*bqf.beta;

            elseif strcmp(filter_type, "Non-Constant")
                bqf.theta = (2*pi*bqf.fc/bqf.fs);
                mu = 10^(bqf.gain/20);
                stag = 4/(1+mu);
                bqf.beta = 0.5*((1-stag*tan(bqf.theta/(2*bqf.Q)))...
                    /(1+stag*tan(bqf.theta/(2*bqf.Q))));
                bqf.gamma = (0.5 + bqf.beta)*cos(bqf.theta);
                bqf.a0 = 0.5 - bqf.beta;
                bqf.a1 = 0;
                bqf.a2 = -1*(0.5-bqf.beta);
                bqf.b1 = -2*bqf.gamma;
                bqf.b2 = 2*bqf.beta;

            elseif strcmp(filter_type, "Constant")
                bqf.theta = 2 * pi * bqf.fc / bqf.fs;
                alpha = sin(bqf.theta) / (2 * bqf.Q);
                A = 10^(bqf.gain / 40);  
                norm = 1 + alpha / A;
                bqf.a0 = (1 + alpha * A) / norm;
                bqf.a1 = -2 * cos(bqf.theta) / norm;
                bqf.a2 = (1 - alpha * A) / norm;
                bqf.b1 = -2 * cos(bqf.theta) / norm;
                bqf.b2 = (1 - alpha / A) / norm;
            end
        end
        function filtered_signal = process(bqf, x)
            N = length(x(:,:));
            filtered_signal = zeros(N,1);
            for n = 1:N
                if n == 1
                    filtered_signal(n,1) = bqf.a0 * x(n,1);
                elseif n == 2
                    filtered_signal(n,1) = bqf.a0 * x(n,1) + bqf.a1 * x(n-1,1) ...
                        - bqf.b1 * filtered_signal(n-1,1);
                else
                    filtered_signal(n,1) = bqf.a0 * x(n,1) + bqf.a1 * x(n-1,1) ...
                        + bqf.a2 * x(n-2,1) - bqf.b1 * filtered_signal(n-1,1)...
                        - bqf.b2 * filtered_signal(n-2,1);
                end
            end
        end
    end

    methods (Static)
        % Static method
        % function output = staticMethod(input)
        %     output = input * 2;
        % end
        % Example method
    end
end
