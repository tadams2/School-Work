import sys
#'max' weight to be used in the UCS
MAX = 9999999999999
def DFS(frontier, srcNode, destNode, path = []):
    #put the start node in the path
    path = path + [srcNode]
    #if we are at the destination return the path
    if srcNode == destNode:
        return path
    
    #for each connected node to the srcNode
    for node in frontier[srcNode]:
        #if the node is not in the path
        if node not in path:
            #get the next extension tothe path recursively
            nextPath = DFS(frontier, node, destNode, path)
            #if next path is empty then we hit a dead end and return back up
            if nextPath:
                return nextPath
    #return none if no path is found
    return None

def BFS(frontier, srcNode, destNode):
    #a queue of the current paths
    pathQueue = []
    #add the source node into the path to start
    pathQueue.append(srcNode)
    #while there are still paths to explore
    while pathQueue:
        #remove the first path and explore it
        path = pathQueue.pop(0)
        #get the last element of the path
        node = path[-1]
        #if we are at the end return the path that found it
        if node == destNode:
            return path

        #expand the current node
        for nextNodes in frontier[node]:
            #turn the dictionary of nextNodes into a list
            nextPath = list(path)
            #append the next nodes into a new path to be added next
            nextPath.append(nextNodes)
            #append the newly built path to the queue
            pathQueue.append(nextPath)



def UCS(frontier, srcNode, destNode, visited = [], distanceDict = {}, prev = {}):
    
    #if the source node is the destination node
    if srcNode == destNode:
        #clear the path 
        path = []
        #BACK TRACKING
        while destNode:
            #add the destination node to the path
            path.append(destNode)
            #take the previous node and make it the destNode
            destNode = prev.get(destNode, None)
        #reverse the path (took me forever i couldnt just use .reverse() ;_;)
        return path[::-1]
    #only used on the first pass to set up the distance dictionary
    if not visited:
        #sets the first node to be distance 0
        distanceDict[srcNode] = 0
    #expands the current node
    for node in frontier[srcNode]:
        #if the node hasnt been visited
        if node not in visited:
            #gets the nodes distance to its neighbor
            distanceToNeighbor = distanceDict.get(node, MAX)
            #checks the possible distance by taking the current distance and adding the distance to the next node
            possibleDistance = distanceDict[srcNode] + frontier[srcNode][node]
            #if the possible distance is less than the distance to the neighbor
            if possibleDistance < distanceToNeighbor:
                #create (or update) distance dict with the new possible distance
                distanceDict[node] = possibleDistance
                #updates the previous{} by adding the current node as the key and its parent as the srcNode
                prev[node] = srcNode
    #add the current node into the visited list
    visited.append(srcNode)
    #find all the next unvisited nodes as long as they are not in the list of visited nodes
    #(i definitely found some help to figure out this list comprehension and holy crap they are powerful)
    unvisitedNodes = dict((k, distanceDict.get(k, MAX)) for k in frontier if k not in visited)
    
    #find the minimun value out of the unvisited nodes 
    nextNode = min(unvisitedNodes, key = unvisitedNodes.get)

    #call UCS on the next node (found from the previous min)
    return UCS(frontier, nextNode, destNode, visited, distanceDict, prev)




def main(argv):
    inputFile = argv[1]
    outputFile = argv[2]
    startNode = argv[3]
    endNode = argv[4]
    searchType = argv[5]
    frontier = {}

    openFile = open(inputFile, "r")
    outFile = open(outputFile, "w")

    nodeList = []

    for line in openFile:
        #inputs[0] src node, inputs[1] dest node, inputs[2] = weight
        inputs = line.split()
        inputs[2] = int(inputs[2])
        if inputs[0] not in nodeList:
            nodeList.append(inputs[0])

        if inputs[1] not in nodeList:
            nodeList.append(inputs[1])

        #if the src node is not in the frontier place 
        if ((inputs[0] in frontier) == False):
            #creates the src node in the frontier with the current dest node and weight
            frontier[inputs[0]] = {inputs[1] : inputs[2]}

        else:
            #if the src node exists add a new dest node with weight
            frontier[inputs[0]][inputs[1]] = inputs[2]
    
    #completeing the frontier for dead end nodes
    for node in nodeList:
        if node not in frontier:
            frontier[node] = {}

    print (frontier)

    if searchType == "BFS":
        outList = BFS(frontier, startNode, endNode)
        for pElement in outList:
            outFile.write(pElement + "\n")

    elif searchType == "DFS":
        outList = DFS(frontier, startNode, endNode)
        for pElement in outList:
            outFile.write(pElement + "\n")

    elif searchType == "UCS":
        outList = UCS(frontier, startNode, endNode)
        for pElement in outList:
            outFile.write(pElement + "\n")


main(sys.argv)
