function mse = mean_square_error( d )
    [r c] = size(d);
    cnt=0;
    total=0.0;
    %row means
    lcl_total=0.0;
    for i=1:r
        rr = d(i,:);
        rr = rr( find(rr ~= 0));
        if numel(rr) > 0
            rowmeans(i) = mean(rr);
            lcl_total = lcl_total + sum(rr);
            cnt = cnt+numel(rr);
        else
            rowmeans(i) = NaN;
        end 
    end
    % col means
    m = lcl_total / cnt;
    for i=1:c
        rr = d(:,i);
        rr = rr( find(rr ~= 0));
        if numel(rr) > 0
            colmeans(i) = mean(rr);
        else
            colmeans(i) = NaN;
        end 
    end

    
    for i=1:r
        rr = d(i,:);
        rr = rr( find(rr ~= 0));
        if numel(rr) > 0
            sub = rr- rowmeans(i);
            for j=1:c
                if ~isnan(colmeans(j))
                    sub = sub - colmeans(j)+m;
                end
            end
            sub = sub .^2;
            total = total + sum(sub);
        end
    end
    mse = total/cnt;

end
