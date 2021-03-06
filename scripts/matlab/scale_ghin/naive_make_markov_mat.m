function [ markov_mat] = naive_make_markov_mat(GI,GJ,psi_gi,psi_gj)
    markov_mat = zeros((GI*GJ)+1,(GI*GJ)+1); % last state is transition to absorbing state
    icnt=1;

    for i=1:GI*GJ
            jcnt=1;
            fprintf('\nj %d of %d ',icnt,GI*GJ);
        for j=1:GI*GJ
           [iii,jjj]=map_to_pair(jcnt,GI,GJ);
           [ii,jj] = map_to_pair(icnt,GI,GJ);
           if i == j
            markov_mat(i,j)=0;
           else
              markov_mat(i,j) =  prob_trans(ii,jj,iii,jjj,psi_gi,psi_gj,GI,GJ);
           end
  
           %fprintf('\nMapping coords %d,%d to %d,%d',ix,iy,jx,jy);
           jcnt=jcnt+1; 
        end
        icnt=icnt+1;
    end
    markov_mat((GI*GJ)+1,1:GI*GJ) = 0;
     markov_mat((GI*GJ)+1,(GI*GJ)+1) = 1;
     % now place absorbing state probablities
     for i=1:GI*GJ
        markov_mat(i,(GI*GJ)+1) = 1-sum(markov_mat(i,(1:GI*GJ)));
     end
    fprintf('\n');

end

function [xx,yy] = map_to_pair(x,X,Y)

    xx= idivide(int32(x),int32(Y),'ceil');
    yy=  x-((xx-1)*Y);
end

function [pt] = prob_trans(i,j,ii,jj,psi_gi,psi_gj,GI,GJ)
    pt = prob_add(ii-i,i,j,psi_gi,GI,GJ) * prob_add(jj-j,j,i,psi_gj,GJ,GI);


end

function [pk] = prob_add(k,Gi,Gj,psi_gi,GI,GJ)
    prior = 1-hygepdf(0,GJ,Gj,psi_gi); % compute probablity of single link
    pk = binopdf(k,GI-Gi,prior);
end


