py_dev folder provides python source code related project for SPIR

Content Description:

+--src: source code
	--RS232_module:RS232 communication function, such as detault configuration, open and close port, read data
	--plot_animation:plot animation function for sensor data visualization
	--monitor_sensor: provide command line interface for user to monitor sensor
					  1)read data from sensor
					  2)plot animation to monitor sensor data
					  3)save sensor data log file

Usage Description:					  
	---Online mode:
		a)	Read in operation command through interface
		b)	Collect real-time data from sensor and plot animation chart 
		c)	Save collected data as log files for offline analysis
		d)	If input "--" as sensor id, then will plot all 8 channels on one chart. Otherwise, only plot specified sensor animation chart.
		Sample command:
			python monitor_sensor.py --online 1
			python monitor_sensor.py --online --
			python monitor_sensor.py --online 1 –log

	---Offline mode:
		a)	Perform offline analysis by plotting static lines based on log data. 
		b)	If input "--" as sensor id, then will plot all 8 channels on one chart. Otherwise, only plot specified sensor animation chart.
		Sample command:
			python monitor_sensor.py --offline 1 2017-05-07-17-13-06.log
			python monitor_sensor.py --offline -- 2017-05-07-17-13-06.log

