# Nginx Cheatsheet

## -c Option

```
nginx -c filename
```

Specify which configuration file NGINX should use instead of the 
default.

Default is **/etc/nginx/nginx.conf**

My default is something like this (for file sharing):

```
user nginx nginx;

worker_processes 4;

events {
	worker_connections 1024;
}


http {

	sendfile on;
	gzip on;
	gzip_comp_level  2;

	server_tokens off;

    server {
		listen	172.16.2.163:9090;

		access_log /home/shared/lol.log;
		error_log /home/shared/error.log;

		location /psoft {
			autoindex	on;
    		root /home/shared;
		}

    }
}
```

some performance modifications:

```
user nginx nginx;

worker_processes 4;
worker_cpu_affinity 1000 0100 0010 0001;

events {
	worker_connections 1024;
	use epoll;
}


http {

	sendfile on;
	gzip on;
	gzip_comp_level  2;

	server_tokens off;

    server {
		listen	172.16.2.163:9090;

		access_log /home/shared/lol.log;
		error_log /home/shared/error.log;

		location /psoft {
			autoindex	on;
    		root /home/shared;
		}

    }
}
```

**user nginx nginx;** start worker processes as user **nginx**
and group **nginx**

(you should create them):

```
groupadd nginx
useradd -g nginx nginx
```

**server_tokens off** disable displaying nginx version

**autoindex on** generates index file automatically.

**location /psoft** will display files in **/home/shared/psoft**

**sendfile on**:

By default, NGINX handles file transmission itself and copies the file 
into the buffer before sending it. Enabling the sendfile directive will
eliminate the step of copying the data into the buffer and enables 
direct copying data from one file descriptor to another.

**gzip on** Enables gzipping of responses.

## -t Option

```
nginx -t
```

Don’t run, just test the configuration file. NGINX checks 
configuration for correct syntax and then try to open files referred 
in configuration.


## -s Option


**nginx –s quit** Stops the daemon gracefully (using the QUIT signal)

**Quit** is a graceful shutdown. Nginx **finishes serving the open 
connections** before shutdown.

**nginx –s stop** Stops the daemon immediately (using the TERM signal)

**Stop** is a quick shutdown where is **terminates** in between 
serving the connection


**nginx –s reopen** Reopens the log files
**nginx –s reload** Reloads the configuration

Note that when starting the daemon, stopping it, or performing any of 
the preceding operations, the configuration file is first parsed and 
verified. If the configuration is invalid, whatever command you have 
submitted will fail, even when trying to stop the daemon. In other 
words, in some cases you will not be able to even stop Nginx if
the configuration file is invalid.

An alternate way to terminate the process, in desperate cases only, 
is to use the **kill** or **killall** commands with root privileges:

```
# killall nginx
```


