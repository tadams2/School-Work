# -*- coding: utf-8 -*-
"""
Created on Wed May 25 11:39:43 2016

@author: Ted
skeleton written by Chris Piech from Standford University
Sourced here: http://stanford.edu/~cpiech/cs221/handouts/kmeans.html
"""

import numpy as np
from sys import argv
import random
import matplotlib.pyplot as plt
import matplotlib.colors as colors


MAX_ITERATIONS = 15

def kmeans(points, k):
    
    centroids = random.sample(points, k)

    allColors = list(colors.cnames.keys())

    iterations = 0
    oldCentroids = None

    while not shouldStop(oldCentroids, centroids, iterations):
        oldCentroids = centroids
        iterations += 1
        
        #we need numpy arrays to do some cool linalg stuff
        points = np.array(points)
        centroids = np.array(centroids)
        labels = getLabels(points, centroids)
        
        centroids = getCentroids(points, labels, k)
    #plotting centroids as a red star
    x, y = zip(*centroids)
    plt.scatter(x,y, marker = '*', color = 'r', s = 80)
    
    #life is a coloring book so lets put colors on stuff
    counter = 0
    for centroid in labels.keys():
        for point in labels[centroid]:
                plt.scatter(point[0], point[1], color = allColors[counter])
       
       #6 was chosen to avoid white, white is apparantly some multiple of 5
        counter += 6

    print (iterations)
    return centroids

def shouldStop(oldCentroids, centroids, iterations):

    #making sure everything is a list to compare it easier
    centroids = np.array(centroids)
    centroids = centroids.tolist()
    oldCentroids = np.array(oldCentroids)
    oldCentroids = oldCentroids.tolist()
    
    if iterations > MAX_ITERATIONS or centroids == oldCentroids:
        return True
    return False

def getLabels(points, centroids):
    clusters = {}
   
    # sourced from https://datasciencelab.wordpress.com/2013/12/12/clustering-with-k-means-in-python/

    for point in points:
        #takes each point and looks at the distance between it and each centroid
        #the min() will take the smallest one of each
        closest = min([(i[0], np.linalg.norm(point-centroids[i[0]])) \
                    for i in enumerate(centroids)], key=lambda t:t[1])[0]
                        
        #add the closest centroid if it exists, sometimes a centroid will have 0 points
        #so we need the try/except
        try:
            clusters[closest].append(point)
        except KeyError:
            clusters[closest] = [point]
    return clusters




def getCentroids(points, labels, k):
    newClusters = []
    #get the middle of each current cluster and return them for new clusters
    #also adapted from https://datasciencelab.wordpress.com/2013/12/12/clustering-with-k-means-in-python/
    keys = sorted(labels.keys())
    for k in keys:
        newClusters.append(np.mean(labels[k], axis = 0))
    return newClusters

def main(argv):
    
    
    numClusters = argv[1]
    numClusters = int(numClusters)    
    filename = argv[2]
    
    openfile = open(filename, 'r')
    
    points = []
    for line in openfile:
        line = line.strip()
        
        x, y = line.split()
        
        points.append([int(x), int(y)])
    

    
    kmeans(points, numClusters)
    plt.show()
    
main(argv)