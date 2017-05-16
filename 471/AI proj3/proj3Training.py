# Import datasets, classifiers and performance metrics
from sklearn import datasets, svm, metrics
import cv2
from sklearn.externals import joblib

datasetTest = [[],[]]


#adding all of the data into the dataset with their targets 
for i in range(0, 70):
    fileName = ("%02d"%int(i + 2))
    filePath = "C:/Users/Ted/Downloads/01-2016-05-23/01/" + fileName + ".jpg"
    imgTest = cv2.imread(filePath, 0)
    imgTest= imgTest.flatten()
    datasetTest[0].append(imgTest)
    datasetTest[1].append(1)
    
    fileName = ("%02d"%int(i + 1))
    filePath = "C:/Users/Ted/Downloads/02-2016-05-23/02/" + fileName + ".jpg"
    imgTest = cv2.imread(filePath, 0)
    imgTest= imgTest.flatten() 
    datasetTest[0].append(imgTest)
    datasetTest[1].append(2)
    
    fileName = ("%02d"%int(i + 1))
    filePath = "C:/Users/Ted/Downloads/03-2016-05-23/03/" + fileName + ".jpg"
    imgTest = cv2.imread(filePath, 0)
    imgTest= imgTest.flatten()
    datasetTest[0].append(imgTest)
    datasetTest[1].append(3)
    
    fileName = ("%02d"%int(i + 1))
    filePath = "C:/Users/Ted/Downloads/04-2016-05-23/04/" + fileName + ".jpg"
    imgTest = cv2.imread(filePath, 0)
    imgTest= imgTest.flatten()
    datasetTest[0].append(imgTest)
    datasetTest[1].append(4)
    
    fileName = ("%02d"%int(i + 1))
    filePath = "C:/Users/Ted/Downloads/05-2016-05-23/05/" + fileName + ".jpg"
    imgTest = cv2.imread(filePath, 0)
    imgTest= imgTest.flatten()
    datasetTest[0].append(imgTest)
    datasetTest[1].append(5)
    


#92%
#classifier = svm.SVC(gamma=0.0000001)

#.9257
classifier = svm.SVC(gamma=0.000000096)


classifier.fit(datasetTest[0][:175], datasetTest[1][:175])

predicted = classifier.predict(datasetTest[0][175:])
expected = datasetTest[1][175:]

total = 0
correct = 0
for i in range(len(predicted)):
    if predicted[i] == expected[i]:
        correct += 1
    total += 1
print("correct:", correct)
print("out of", total, "samples")
print("accurary:",correct / total)
#print(predicted)

joblib.dump(classifier, 'classifier.pkl')

