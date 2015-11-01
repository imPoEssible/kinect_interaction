import cv2
import numpy as np

face_cascade = cv2.CascadeClassifier('/home/shruti/POE/face_detection/haarcascade_frontalface_alt.xml')
mouth_cascade = cv2.CascadeClassifier('/home/shruti/POE/face_detection/haarcascade_smile.xml')

cap = cv2.VideoCapture(0)

kernel = np.ones((21,21),'uint8')

while(True):
    ret, frame = cap.read() 
    img = frame
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) 
    
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2, minSize=(20,20))
     
    for (x,y,w,h) in faces:
        cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255),2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = frame[y:y+h, x:x+w]
        
        mouth = mouth_cascade.detectMultiScale(roi_gray, scaleFactor=1.7, minNeighbors=20, minSize=(19,19), flags=cv2.cv.CV_HAAR_SCALE_IMAGE)
        
        for (mp,mq,mr,ms) in mouth:
            cv2.rectangle(roi_color,(mp,mq),(mp+mr,mq+ms), (255,0,0),1)
     
    cv2.imshow("output", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()