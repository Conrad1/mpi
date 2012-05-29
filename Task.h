/* 
 * File:   Task.h
 * Author: matt
 *
 * Created on 27 May 2012, 6:52 PM
 */

#ifndef TASK_H
#define	TASK_H

class ResultSender;

class Task {
public:
    Task();
    Task(const Task& orig);
    virtual ~Task();
    
    virtual void run(ResultSender & resultSender) = 0;
private:

};

#endif	/* TASK_H */

