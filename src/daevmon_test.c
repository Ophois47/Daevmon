#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


static void daevmon() {
	pid_t process_id = 0;
	pid_t session_id = 0;

	// Create Child Process
	process_id = fork();

	// Indication of fork() Failure
	if (process_id < 0) {
		printf("Fork Failure...\n");
		exit(1);
	}

	// Parent Process. To Be KILLED
	if (process_id > 0) {
		printf("Process_ID of Child Process: %d\n", process_id);
		exit(0);
	}

	// Set New File Permissions
	umask(0);

	// Set New Session, Child Process Becomes Session Leader
	session_id = setsid();

	if (session_id < 0) {
		exit(1);
	}

	// Change Current Working Directory to Root
	chdir("/tmp");

	// TODO: Make Better
	// Catch, Ignore, and Handle Signals
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	// Close stdin, stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// To Be Extra Special Sure
	int file_desc;
	for (file_desc = sysconf(_SC_OPEN_MAX); file_desc >= 0; file_desc--) {
		close(file_desc);
	}

	openlog("Daevmon", LOG_PID, LOG_DAEMON);
}

int main() {
	daevmon();
	
	while (1) {
		// Do Tasks Here
		syslog(LOG_NOTICE, "Daevmon Begun!\n\n");
		sleep(5);
		break;
	}

	syslog(LOG_NOTICE, "Process Terminated!\n");
	closelog();

	return 0;
}