
close all
disp('Opening port..')
% you might have to change the port number
com = CreateSerialConnection('COM19',512000);

%InputBufferSize=100;
total_samples=301; % the script reads this many samples and then stops.
power=16; % # of powerpins on board
ground=7; % # of groundpins on board
buf=(power*ground*2)+9; % size of buffer = grid points x 2 (upper and lower bound) + header (4), timestamp (4) and checksum (1)
data=zeros(power,ground,total_samples); % the pressure map data, arranged in a grid. the third dimension is time.
timestamp=zeros(total_samples,1); % time values

disp('Reading data')



k=1;
tic;

    %% BEFORE RUNNING%%
    %%CHECK: participant number
    %%CHECK: randomized list of gestures
    
    %BLOCK 1%
    fileName = 'test44444444.csv'
    %fileName ='P10-S167A-B1-scratch.csv';
    %fileName ='P10-S167A-B1-poke.csv';
    %fileName ='P10-S167A-B1-constant.csv';
    %fileName ='P10-S167A-B1-slap.csv';
    %fileName ='P10-S167A-B1-stroke.csv';
    %fileName ='P10-S167A-B1-notouch.csv';
    %fileName ='P10-S167A-B1-tickle.csv';
    %fileName ='P10-S167A-B1-tap.csv';
    %fileName ='P10-S167A-B1-pat.csv'; 
    %fileName ='P10-S167A-B1-pinch.csv';
    %fileName ='P10-S167A-B1-rub.csv';
   
    
    %BLOCK 2%
    %fileName ='P10-S167A-B2-slap.csv';
    %fileName ='P10-S167A-B2-poke.csv';
    %fileName ='P10-S167A-B2-tap.csv';
    %fileName ='P10-S167A-B2-rub.csv';
    %fileName ='P10-S167A-B2-stroke.csv';
    %fileName ='P10-S167A-B2-scratch.csv';
    %fileName ='P10-S167A-B2-constant.csv';
    %fileName ='P10-S167A-B2-notouch.csv';
    %fileName ='P10-S167A-B2-pat.csv';
    %fileName ='P10-S167A-B2-tickle.csv';
    %fileName ='P10-S167A-B2-pinch.csv';


while(k<total_samples)
   % disp('in loop for collection')
    % wait until all data is available
    while (get(com,'BytesAvailable')<buf)
       
    end
     %disp('com set')
    % check for the header
    %disp(fread(com,4)==[255 255 255 255]')
    while (sum(fread(com,4)==[255 255 255 255]')<4)
        
    end
    % disp('computed sum')
    % wait for the rest of the data
    while (get(com,'BytesAvailable')<buf-4)
    end
    %disp('com again')
    % read the timestamp (4 bytes)
    t=fread(com,1,'uint32');
    timestamp(k)=t;
    %disp(timestamp(k))
    % read the pressure map (powerpins x groundpins = bytes)
    value=fread(com,[ground power],'uint16');
    %disp(value)
    %display(t);
    diff=1024-value;
    %disp(diff)
   
    
    % read the lowest 8 bits of the checksum
    true_checksum=fread(com,1,'uint8');
    
    % calculate the checksum, check if it is correct
    calc_checksum=sum(sum(value))+t;
    calc_checksum=mod(calc_checksum,256); 
    
    if (true_checksum ~= calc_checksum)
        disp('Bad data!')
        continue;
    end
    
    
    % if everything's fine, put data in arrays
    value=value';
    %value=value';
    %data(:,:,k)=value;
    data=value(:,1:ground);

    k=k+1;
    
    dlmwrite(fileName, data, '-append');
    


    % draw the pressure map if we have time
    if get(com,'BytesAvailable')<buf 
        imagesc(data,[0 900])
        %colormap(gray)
        axis equal;
        axis off;
        pause(0.001)
        
       disp(data);
    %else
     %   disp('No time to draw!');
    end
    
    
end

fileID = fopen(fileName,'at');
fmt = '\nduration: %d\n';
fprintf(fileID,fmt, toc);
fclose(fileID);

toc;
fclose(com);
delete(com);
