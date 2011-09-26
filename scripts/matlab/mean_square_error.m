function mse = mean_square_error( d )
    [r c] = size(d);
    total=0.0;
    m = mean(d(:));
    rowmeans = mean(d');
    colmeans = mean(d);
    
    for i=1:r
        sub = d(i,:) - rowmeans(i);
        for j=1:c
            sub = sub - colmeans(j)+m;
        end
        %sub = sub + m
        sub = sub .^2;
        total = total + sum(sub);
    end
    mse = total/(r*c);

end

