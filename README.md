
WA Analyzer
===========

A analyzer app of WhatsApp for smartphones with Symbian OS developed using Qt and C++.

Analyze logs files to display and export the data of the communications in lists, reports and statistics.


Screenshots
-----------

![Alt text](https://raw.githubusercontent.com/jh2odo/wa-analyzer/master/screens/screen.png)


Downloads Files
---------------

You need to uninstall the Nokia Store version manually because this are a self-signed version and will not replace the Nokia Store version. 

Latest stable release for Symbian^1(5th edition) and Symbian^3(Anna and Belle) Operating System: 

https://github.com/jh2odo/wa-analyzer/releases

- WAAnalyzer_unsigned_anna_v2_2_0.sis
- WAAnalyzer_unsigned_s60_5_v2_2_0.sis

Features
--------

- Parser logs (debug) files of WhatsApp App to get data of the comunications and to analyze them
- Limit notifications 7 days
- Limit stats 30 days
- Show notifications messages received and sent with the details:
  + From/To: the contact the message.
  + Type: text, image, audio, video, contact and location
  + Source: private or group (contact creator)
  + Length: length of message
- Support contacts from phone native
- Module statistics
  + Charts: Pie, Histogramm and Graphic
- Languages: English and Spanish, native; Portuguese, French, German, Italian and Russian, Google Translate
- Phones (tested): Nokia N8, Nokia C7 and Nokia 5800 XpressMusic
- Network access: No required
- Compatible with WhatsApp 2.8.x and 2.9.x (last)
- Recommended: WhatsApp 2.8.22
- Symbian^1(5th edition) and Symbian^3(Anna and Belle) Operating System

Technologies
------------

- Symbian^1(5th edition) and Symbian^3(Anna and Belle) Operating System
- QT Mobile Aplication (Widget)
- Qt 4.7.3(s60_5) and Qt 4.7.4(anna)
- Qt Mobility (Contacts, feedback and systeminfo)
- C++
- zlib : http://www.zlib.net/
- QsKineticScroller : https://bitbucket.org/razvanpetru/qt-components
- NightCharts : https://code.google.com/p/shopping-lists-organizer/
- Network access is not necessary to use the app

Description Internal
--------------------

- Gui screen orientation lock portrait with resolution 640 � 360 pixels
- The application gets data from text files and gz.
- Uses reading mode phone contacts.
- Processes and combines the data to display a report about the datails of messages in a list.
- Serialize and store logs processed in directory dat (performance,less system resources)
- Compatible with WhatsApp 2.8.x and 2.9.x (last)
- Recommended WhatsApp 2.8.22

License
-------

WA Analyzer license:

	The MIT License (MIT)

	Copyright (c) 2012 Jose Aguado (jh2odo)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE

Zlib library (libs/zlib125-dll.zip):

	Copyright (C) 1995-2010 Jean-loup Gailly and Mark Adler

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

QsKineticScroller library for Qt license (util/QsKineticScroller.h):

	Copyright (c) 2011, Razvan Petru
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this
	  list of conditions and the following disclaimer in the documentation and/or other
	  materials provided with the distribution.
	* The name of the contributors may not be used to endorse or promote products
	 derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
	OF THE POSSIBILITY OF SUCH DAMAGE.
	
Modified version of NightCharts library (util/charts.h):	

	NightCharts
	Copyright (C) 2010 by Alexander A. Avdonin, Artem N. Ivanov / ITGears Co.
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	Please contact gordos.kund@gmail.com with any questions on this license.
	
	
