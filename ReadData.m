clc;
clear all;

%Read data from .txt file
fid = fopen('DATA0.txt');
textData = textscan(fid, '%n%n%n%n%n%n%n%n%n%n%n%n%n%n', 100000000000, 'headerlines', 1, 'delimiter', ',');
fclose(fid)

%Plot data

for i = 1:11
    figure(i)
    plot(textData{1}, textData{i+1})
    switch i
        case 1
            title('Time vs. Pressure')
            xlabel('Time')
            ylabel('Pressure')
        case 2
            title('Time vs. Altitude')
            xlabel('Time')
            ylabel('Altitude')
        case 3
            title('Time vs Yaw Angle')
            xlabel('Time')
            ylabel('Yaw Angle')
        case 4
            title('Time vs. Roll Angle')
            xlabel('Time')
            ylabel('Roll Angle')
        case 5
            title('Time vs. Pitch Angle')
            xlabel('Time')
            ylabel('Pitch Angle')
        case 6
            title('Time vs. Longatude')
            xlabel('Time')
            ylabel('Longatude')
        case 7
            title('Time vs. Latitude')
            xlabel('Time')
            ylabel('Latitude')
        case 8
            title('Time vs. Elevator Servo Angle')
            xlabel('Time')
            ylabel('Elevator Servo Angle')
        case 9
            title('Time vs. Left Aieleron Servo Angle')
            xlabel('Time')
            ylabel('Left Aileron Servo Angle')
        case 10
            title('Time vs. Right Aieleron Servo Angle')
            xlabel('Time')
            ylabel('Right Aileron Servo Angle')
        case 11
            title('Time vs. Rudder Servo Angle')
            xlabel('Time')
            ylabel('Rudder Servo Angle')
        case 12
            title('Time vs. Desired Pitch')
            xlabel('Time')
            ylabel('Desired Pitch')
        case 13
            title('Time vs. Desired Roll')
            xlabel('Time')
            ylabel('Desired Roll')
    end     
end

figure (14)
plot(textData{6}, textData{7})
title('Longatude vs. Latitude')
figure(15)
plot(textData{1}, textData{5})
hold on 
plot(textData{1}, textData{13})
title('Desired Roll Compared to Actual Roll');
xlabel('Time')

    


