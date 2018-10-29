# Microsoft Cognitive Services C++ SDK

This repo is not an official SDK. The project is a wrapper to use Microsoft Cognitive Services in standard C++ as is currently not supported.

The project provide full support to Computer Vision API. A wrapper to **gphoto2** will provide the full control digital cameras (**DSLR**), getting the raw shot and making manipulations with Computer Vision API.

### Compile examples

***
Because of the support to **gphoto2** you need some packages are required. In Ubuntu you can install:

* apt-get install build-essential
* apt-get install libcurl4-openssl-dev (libcurl)
* apt-get install libjsoncpp-dev (libjson)
* apt-get install pkg-config
* apt-get install gphoto2
* apt-get install libgphoto2-dev

Into path _examples/_ 
`cmake .`

if all packages are found you can run `make -j 4`

one compiled all examples you can test:


**Analyze Image**

`analyze -i https://upload.wikimedia.org/wikipedia/commons/d/de/Nokota_Horses_cropped.jpg -s=subscriptionKey`

`analyze -i images/sample01.jpg -s=subscriptionKey`


**Describe Image**

`describe -i images/sample01.jpg -s=subscriptionKey`

`describe -i https://upload.wikimedia.org/wikipedia/commons/d/de/Nokota_Horses_cropped.jpg -s=subscriptionKey`


**Tag Image**

`tag -i images/sample02.jpg -s=subscriptionKey`

`tag -i https://media.timeout.com/images/103835606/630/472/image.jpg -s=subscriptionKey`


**Face Detect**

`face -i https://i.ytimg.com/vi/0rNN65r7ST4/maxresdefault.jpg -s=visionKey`

`face -i images/sample07.jpg -s=visionKey`


**OCR**

`ocr -i images/sample03.jpg -s=subscriptionKey`

`ocr -i https://www.w3.org/TR/2009/REC-SVGTiny12-20090303/examples/textArea01.png -s=subscriptionKey`

**Recognize Domain Specific Content**

`recognize-domain-specific-content -model landmarks -i images/sample06.jpg -s=subscriptionKey`

`recognize-domain-specific-content -model landmarks -i http://www.turismo.it/typo3temp/pics/87f6c362f9.jpg -s=subscriptionKey`

`recognize-domain-specific-content -model celebrities -i https://ichef.bbci.co.uk/images/ic/960x540/p01bqlx8.jpg -s=subscriptionKey`


**Recognize Text**

`recognize-text -i images/sample03.jpg -s=subscriptionKey`

`recognize-text -i https://inhabitat.com/wp-content/blogs.dir/1/files/2015/01/Malala-handwritten-tweet.jpg -s=subscriptionKey`

**Get Thumbnail**

`thumbnail -i images/sample02.jpg -s=subscriptionKey`

`thumbnail -i https://media.wired.com/photos/5aa18e3edc9df6500b90747b/master/pass/CarRoundup-Mclaren.jpg -s=subscriptionKey`

**Custom Vision**

`custom-vision -i images/sample08.jpg -projectid 09f83353-5c2f-417d-b794-2bfc162dd684 -s=predictionKey`

`custom-vision -i https://www.kmart.com.au/wcsstore/Kmart/images/ncatalog/f/7/42171027-1-f.jpg -projectid <YOUR PROJECT ID> -s=predictionKey`


**Camera AI**

`camera-ai -s=visionKey -a=face`

`camera-ai -s=predictionKey -a=custom-vision -projectid=<YOUR PROJECT ID>`

`camera-ai -s=subscriptionKey -a=describe`

`camera-ai -s=subscriptionKey -a=analyze`

`camera-ai -s=subscriptionKey -a=tags`