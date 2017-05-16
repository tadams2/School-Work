

from sklearn.externals import joblib
from sklearn import datasets, svm, metrics
from sys import argv
import cv2

filename = argv[1]
imgTest = cv2.imread(filename, 0)

imgTest= imgTest.flatten()
classifier = joblib.load('classifier.pkl')

class2pic = {1 : "Smile", 2 : "Hat", 3 : "Hash", 4: "Heart", 4: "Dollar"}
predicted = classifier.predict(imgTest)

print (class2pic[(predicted[0])])


