function [zf,idf,zn,table]= data_associate_known(z, idz, table, Nf)
% 

zf= []; zn= [];
idf= []; idn= [];

% find associations (zf) and new features (zn)
for i=1:length(idz)
    ii= idz(i);
    if table(ii) == 0 % new feature
        zn= [zn z(:,i)];
        idn= [idn ii];
    else
        zf= [zf z(:,i)];
        idf= [idf table(ii)];
    end
end

% add new feature IDs to lookup table
table(idn)= Nf + (1:size(zn,2)); % add new feature positions to lookup table
