/* 
 * File:   MPIRunner.h
 * Author: matt
 *
 * Created on 27 May 2012, 5:20 PM
 */

#ifndef MPIRUNNER_H
#define	MPIRUNNER_H

#include "Task.h"
#include "Aggregator.h"
#include "CommChannel.h"

class MPIRunner 
{
public:
    MPIRunner(int &argc, char **&argv);
    MPIRunner(const MPIRunner& orig);
    
    bool isMaster() { return _myNode == 0; }
    void startup(Task &task, Aggregator &aggregator);
    
    virtual ~MPIRunner();
    const int node() const { return _myNode; }
private:
    int _myNode;
    int _totalNodes;
    
    void waitToAggregate(Aggregator & aggregator);
 };

#endif	/* MPIRUNNER_H */

