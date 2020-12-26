#include "MyForm.h"
#include "imgProcess.h"
#include <atlstr.h>
#include <math.h>
#include "humoments.h"

#define PI 3.14159265

using namespace ODEV2;

//Restart the app
System::Void MyForm::restartButton_Click(System::Object^ sender, System::EventArgs^ e)
{

	System::Windows::Forms::Application::Restart();
}

//Load Image
System::Void MyForm::loadButton_Click(System::Object^ sender, System::EventArgs^ e)
{

	CString filePath;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		filePath = openFileDialog1->FileName;
		input = (LPCTSTR)filePath;
		image = LoadBMP(imageWidth, imageHeight, imageSize, input);

		pictureBox1->Width = imageWidth;
		pictureBox1->Height = imageHeight;
		pictureBox1->ImageLocation = openFileDialog1->FileName;

		loadControl = TRUE;
	}




}

//Save Image
System::Void MyForm::saveButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (intensityControl)
	{
		BYTE* item = ConvertIntensityToBMP(processedImage, imageWidth, imageHeight, newSize);
		CString Location = "Images//Outs//Out" + processCount + ".bmp";
		output = Location;
		if (SaveBMP(item, imageWidth, imageHeight, newSize, output))
			System::Windows::Forms::MessageBox::Show("Output Image was successfully saved");
		else
			System::Windows::Forms::MessageBox::Show("Error on saving image");

		delete[] item;
		processCount++;
	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the intensity process at least !");
}

//Intensity
System::Void MyForm::intensityButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (loadControl)
	{
		processedImage = ConvertBMPToIntensity(image, imageWidth, imageHeight);

		pictureBox2->Width = pictureBox1->Width;
		pictureBox2->Height = pictureBox1->Height;


		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		Color pixelColor;

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				pixelColor = Color::FromArgb(processedImage[pos], processedImage[pos], processedImage[pos]);
				surface->SetPixel(col, row, pixelColor);
			}

		pictureBox2->Image = surface;
		intensityControl = TRUE;

		/*if (imageSize == imageHeight * imageWidth * 3)
			System::Windows::Forms::MessageBox::Show("evet" + imageWidth + imageSize / imageHeight);
		else
			System::Windows::Forms::MessageBox::Show("hayir");*/


	}
	else
		System::Windows::Forms::MessageBox::Show("You must load the image first !");


}

//Histogram Chart
System::Void MyForm::histogramButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (intensityControl)
	{

		for (int i = 0; i < 256; i++)  //bo� bir histogram dizini olu�turma
			histogram[i] = 0;

		for (int i = 0; i < imageWidth * imageHeight; i++)  //histogram dizinini doldurma
			histogram[(int)processedImage[i]]++;

		chart1->Series["Pixels"]->Points->Clear();

		for (int i = 0; i < 256; i++)  //histogram dizinine ait grafik �izdirme
			chart1->Series["Pixels"]->Points->AddXY(i, histogram[i]);


		histogramControl = TRUE;

		//System::Windows::Forms::MessageBox::Show(imageWidth + " " + imageHeight + " " + imageSize + " "); //to see image values

	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the intensity process first !");

}

//Histogram 3d Chart
System::Void MyForm::histogram3dButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (loadControl)
	{


		for (int i = 0; i < 256 * 256 * 256; i++)  //3dHistogram dizini i�in her bir renk ��lemesini saklayacak 256 taban�nda 3 basamakl� say�lar dizini tan�m� yapt�m.
		{										   //�rne�in beyazlar = 255255255 siyahlar = 0 indisinde saklanacak
			histogram3d[i] = 0;
		}



		for (int i = 0; i < imageSize; i += 3)
		{
			histogram3d[(256 * 256 * (int)image[i + 2]) + (256 * (int)image[i + 1]) + (int)image[i]]++; //256 taban�nda 3 basamakl� say�...
		}


		chart2->Series["Pixels"]->Points->Clear();
		for (int i = 0; i < 256 * 256 * 256; i += 128)   //dizine ait de�erlerin grafi�ini �izme
		{
			chart2->Series["Pixels"]->Points->AddXY(i, histogram3d[i]);
		}

		histogram3dControl = TRUE;

	}
	else
		System::Windows::Forms::MessageBox::Show("You must load the image first !");

}

//K-means, binary image
System::Void MyForm::kmeansButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (histogramControl)
	{
		if (clusterCountMenu->SelectedIndex != -1)
		{



			int centroidCount = clusterCountMenu->SelectedIndex + 2;  //se�ilen s�n�f say�s� kadar centroid isminde histogramdaki x ekseninde gezecek olan indislerin tan�m�
			double* centroid = new double[centroidCount];
			for (int i = 0; i < centroidCount; i++)
			{
				centroid[i] = double(i * 255) / (centroidCount - 1);  //0 ile 255 aras�nda e�it uzakl�kta ba�lang�� de�erleri atama.  �rne�in 2 centroid i�in s�ras�yla 0 ve 255, 3 i�in 0 127 255 
			}
			double* newCentroid = new double[centroidCount];			//yap�lan hesaplar sonucu centroid de�i�kenlerine ait yeni de�erleri saklayacak olan de�i�kenlerin tan�m�
			for (int i = 0; i < centroidCount; i++)
			{
				newCentroid[i] = -1;
			}
			double* sum = new double[centroidCount];   //centroidlere yak�n indisli histogram de�erlerinin o indislerle �arp�m�n�n toplam�

			double* div = new double[centroidCount];   //centroidlere yak�n olan indisli histogram de�erlerinin toplam�, bu iki de�erin oran�ndan orta indis de�erini yani o centroide ait newCentroid de�erini buluyoruz

			double err = 1;

			while (err)
			{
				err = 0;

				for (int i = 0; i < centroidCount; i++)
				{
					sum[i] = 0;
					div[i] = 0;
				}

				for (int i = 0; i < 256; i++)
				{
					double distance = 999;
					int nearest;
					for (int j = 0; j < centroidCount; j++)
						if (Math::Abs(i - centroid[j]) < distance)
						{
							distance = Math::Abs(i - centroid[j]);
							nearest = j;
						}

					sum[nearest] += i * histogram[i];
					div[nearest] += histogram[i];

					for (int i = 0; i < centroidCount; i++)
						if (!div[i]) div[i]++;

					for (int i = 0; i < centroidCount; i++)
						newCentroid[i] = Math::Round((sum[i] / div[i]));

				}


				for (int i = 0; i < centroidCount; i++)  //hata miktar� eski ve yeni centroid de�erlerinin her biri i�in farklar�n�n toplam�, k�saca t�m eski ve yeni centroid de�erleri birbirlerine e�it olmad�k�a bu i�lemler devam edecek
					err += newCentroid[i] - centroid[i];

				for (int i = 0; i < centroidCount; i++)
					centroid[i] = newCentroid[i];


			}

			BYTE* binaryImage = new BYTE[imageWidth * imageHeight];

			Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

			//10 farkl� s�n�f i�in kullan�lacak renk de�erleri
			Color pen1 = Color::Black;
			Color pen2 = Color::White;
			Color pen3 = Color::Purple;
			Color pen4 = Color::Brown;
			Color pen5 = Color::Red;
			Color pen6 = Color::Orange;
			Color pen7 = Color::Pink;
			Color pen8 = Color::Blue;
			Color pen9 = Color::Green;
			Color pen10 = Color::Violet;

			int pos = 0;

			BYTE* palette = new BYTE[centroidCount];     //farkl� renk olan b�lgeleri farkl� de�erlerle doldurmak i�in e�it aral�kta olu�turulmu� dizi, centroid dizisinin ba�lang�� hali ile ayn�
			for (int i = 0; i < centroidCount; i++)
				palette[i] = (i * 255) / (centroidCount - 1);

			for (int row = 0; row < imageHeight; row++)
				for (int col = 0; col < imageWidth; col++)
				{
					pos = row * imageWidth + col;

					int distance = 999;
					int nearest;

					for (int j = 0; j < centroidCount; j++)
						if (Math::Abs(processedImage[pos] - centroid[j]) < distance)
						{
							distance = Math::Abs(processedImage[pos] - centroid[j]);
							nearest = j;
						}

					switch (centroidCount)
					{
					case 2:

						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						break;

					case 3:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else
						{
							binaryImage[pos] = palette[2];
							surface->SetPixel(col, row, pen3);
						}
						break;

					case 4:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						break;

					case 5:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						break;

					case 6:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else if (nearest == 4)
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						else
						{
							binaryImage[pos] = palette[6];
							surface->SetPixel(col, row, pen6);
						}
						break;

					case 7:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else if (nearest == 4)
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						else if (nearest == 5)
						{
							binaryImage[pos] = palette[6];
							surface->SetPixel(col, row, pen6);
						}
						else
						{
							binaryImage[pos] = palette[7];
							surface->SetPixel(col, row, pen7);
						}
						break;

					case 8:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else if (nearest == 4)
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						else if (nearest == 5)
						{
							binaryImage[pos] = palette[6];
							surface->SetPixel(col, row, pen6);
						}
						else if (nearest == 6)
						{
							binaryImage[pos] = palette[7];
							surface->SetPixel(col, row, pen7);
						}
						else
						{
							binaryImage[pos] = palette[8];
							surface->SetPixel(col, row, pen8);
						}
						break;

					case 9:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else if (nearest == 4)
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						else if (nearest == 5)
						{
							binaryImage[pos] = palette[6];
							surface->SetPixel(col, row, pen6);
						}
						else if (nearest == 6)
						{
							binaryImage[pos] = palette[7];
							surface->SetPixel(col, row, pen7);
						}
						else if (nearest == 7)
						{
							binaryImage[pos] = palette[8];
							surface->SetPixel(col, row, pen8);
						}
						else
						{
							binaryImage[pos] = palette[9];
							surface->SetPixel(col, row, pen9);
						}
						break;

					case 10:
						if (nearest == 0)
						{
							binaryImage[pos] = palette[0];
							surface->SetPixel(col, row, pen1);
						}
						else if (nearest == 1)
						{
							binaryImage[pos] = palette[1];
							surface->SetPixel(col, row, pen2);
						}
						else if (nearest == 2)
						{
							binaryImage[pos] = palette[3];
							surface->SetPixel(col, row, pen3);
						}
						else if (nearest == 3)
						{
							binaryImage[pos] = palette[4];
							surface->SetPixel(col, row, pen4);
						}
						else if (nearest == 4)
						{
							binaryImage[pos] = palette[5];
							surface->SetPixel(col, row, pen5);
						}
						else if (nearest == 5)
						{
							binaryImage[pos] = palette[6];
							surface->SetPixel(col, row, pen6);
						}
						else if (nearest == 6)
						{
							binaryImage[pos] = palette[7];
							surface->SetPixel(col, row, pen7);
						}
						else if (nearest == 7)
						{
							binaryImage[pos] = palette[8];
							surface->SetPixel(col, row, pen8);
						}
						else if (nearest == 8)
						{
							binaryImage[pos] = palette[9];
							surface->SetPixel(col, row, pen9);
						}
						else
						{
							binaryImage[pos] = palette[10];
							surface->SetPixel(col, row, pen10);
						}
						break;
					}

					//�l�m for b�lgesi
				}

			/*for(int i=0;i<centroidCount;i++)
			System::Windows::Forms::MessageBox::Show(palette[i] + " ");*/

			pictureBox2->Image = surface;
			processedImage = binaryImage;

			if (centroidCount == 2)             //obje olan yerler 1 ,bo� yerler 0 olmas� i�in. �l�m for b�lgesinde yapmaya cal�� sonra.
				for (int i = 0; i < imageHeight * imageWidth; i++)        //�devin geri kalan k�sm�nda 2 s�n�f kullanaca��m i�in 2 s�n�f k-means a �zel bir for d�ng�s� yazd�m..
				{
					if (!processedImage[i]) processedImage[i] = (BYTE)1;
					else processedImage[i] = (BYTE)0;
				}

			image255 = new unsigned char[imageWidth * imageHeight];           //unsigned char tipinde image dizisi olu�turma
			for (int i = 0; i < imageWidth * imageHeight; i++)
			{
				if (processedImage[i] == 1) image255[i] = (BYTE)255;
				else if (processedImage[i] == 0) image255[i] = (BYTE)0;
			}

			kmeansControl = TRUE;

			/*for (int i = 0; i < 100; i++)
				System::Windows::Forms::MessageBox::Show("" + processedImage[i]);*/

		}
		else
			System::Windows::Forms::MessageBox::Show("You must select the number of clusters first !");


	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the histogram process first !");


}

//Marking the objects
System::Void MyForm::markerButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (kmeansControl)
	{
		markedImage = new int[imageHeight * imageWidth];        //objelerin yerlerinde 0 dan farkl� de�erlerde obje say�s� kadar artacak olan say�lardan olu�an bir obje haritas� dizisi
		for (int i = 0; i < imageHeight * imageWidth; i++)
			markedImage[i] = 0;

		int mark = 0;

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				if (processedImage[pos] && !markedImage[pos])
				{
					mark++;
					Marker(markedImage, pos, mark);     //bir objeyi tamamen i�aretleyene dek recursive olarak �al��acak olan fonksiyon
				}
			}


		/*Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);										//nesnelerin kenarlar�n� k�rm�z� ile g�sterme

		Color redPen = Color::Red;
		for (int row = 2; row < imageHeight - 2; row++)
			for (int col = 2; col < imageWidth - 2; col++)
			{
				int pos = row * imageWidth + col;

				if (markedImage[pos])
				{
					if (!markedImage[pos + 2]) surface->SetPixel(col + 2, row, redPen);
					if (!markedImage[pos - 2]) surface->SetPixel(col - 2, row, redPen);
					if (!markedImage[pos - 2 * imageWidth]) surface->SetPixel(col, row - 2, redPen);
					if (!markedImage[pos + 2 * imageWidth]) surface->SetPixel(col, row + 2, redPen);

				}
			}*/

			//Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);										//nesneleri grinin tonlar�yla g�sterme

			//for (int row = 0; row < imageHeight; row++)
			//	for (int col = 0; col < imageWidth; col++)
			//	{
			//		int pos = row * imageWidth + col;
			//		Color color = Color::FromArgb((markedImage[pos] * 10) % 255, (markedImage[pos] * 10) % 255, (markedImage[pos] * 10) % 255);
			//		surface->SetPixel(col, row, color);
			//	}


		objectCount = mark;
		int window[4];      //objeyi �evreleyen bir pencerenin sol �st ve sa� alt pozisyon bilgilerini tutacak olan dizi...

		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				if (!processedImage[pos]) surface->SetPixel(col, row, Color::White);
				else surface->SetPixel(col, row, Color::Black);
			}

		std::ofstream writer;
		writer.open("objectPositions.txt");

		for (int i = 1; i < objectCount + 1; i++)    //her bir obje i�in pencere de�i�keninin doldurulup de�erlerinin bir txt dosyas�na yaz�lmas� ve bu bilgilere g�re �er�evelerin g�sterimi
		{
			//maximum alabilecekleri de�erler
			window[0] = imageWidth - 1;
			window[1] = imageHeight - 1;
			window[2] = 0;
			window[3] = 0;

			for (int row = 0; row < imageHeight; row++)
				for (int col = 0; col < imageWidth; col++)
				{
					int pos = row * imageWidth + col;

					if (markedImage[pos] == i)
					{

						if (col < window[0])	//x1
							window[0] = col;
						if (row < window[1])	//y1
							window[1] = row;
						if (col > window[2])	//x2
							window[2] = col;
						if (row > window[3])	//y2
							window[3] = row;

					}
				}

			writer << window[0] << " " << window[1] << " " << window[2] << " " << window[3] << " ";

			//bilgilere g�re �er�evelerin g�sterimi
			for (int i = window[0]; i < (window[2] + 1); i++)
			{
				surface->SetPixel(i, window[1], Color::Red);
				surface->SetPixel(i, window[3], Color::Red);
			}
			for (int i = window[1]; i < (window[3] + 1); i++)
			{
				surface->SetPixel(window[0], i, Color::Red);
				surface->SetPixel(window[2], i, Color::Red);
			}
		}

		writer.close();




		pictureBox2->Image = surface;

		markerControl = TRUE;
	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the kmeans process first !");

}

//Recursive Marker Method
Void MyForm::Marker(int* markMap, int position, int mark)
{
	markMap[position] = mark;            //i�aretleme
	//out << position << " ";              //nesnelerin pozisyonlar�n� kaydetme


	if (processedImage[position + 1] && !markMap[position + 1])
		Marker(markMap, position + 1, mark);
	if (processedImage[position - 1] && !markMap[position - 1])
		Marker(markMap, position - 1, mark);
	if (processedImage[position + imageWidth] && !markMap[position + imageWidth])
		Marker(markMap, position + imageWidth, mark);
	if (processedImage[position - imageWidth] && !markMap[position - imageWidth])
		Marker(markMap, position - imageWidth, mark);

}

//Erosion
System::Void MyForm::erosionButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (kmeansControl)
	{
		BYTE* erodedImage = new BYTE[imageHeight * imageWidth];
		for (int i = 0; i < imageHeight * imageWidth; i++)
			erodedImage[i] = 0;


		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				if (!processedImage[pos] || !processedImage[pos + 1] || !processedImage[pos - 1] || !processedImage[pos + imageWidth] || !processedImage[pos - imageWidth])
					erodedImage[pos] = 0;
				else
					erodedImage[pos] = 1;


			}


		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;

				if (erodedImage[pos])
					surface->SetPixel(col, row, Color::Black);
				else
					surface->SetPixel(col, row, Color::White);

			}

		processedImage = erodedImage;
		pictureBox2->Image = surface;
	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the kmeans process first !");
}

//Dilation
System::Void MyForm::dilationButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (kmeansControl)
	{
		BYTE* dilatedImage = new BYTE[imageHeight * imageWidth];
		for (int i = 0; i < imageHeight * imageWidth; i++)
			dilatedImage[i] = 0;


		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				if (processedImage[pos])
				{
					dilatedImage[pos] = 1;
					dilatedImage[pos + 1] = 1;
					dilatedImage[pos - 1] = 1;
					dilatedImage[pos + imageWidth] = 1;
					dilatedImage[pos - imageWidth] = 1;
				}


			}


		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;

				if (dilatedImage[pos])
					surface->SetPixel(col, row, Color::Black);
				else
					surface->SetPixel(col, row, Color::White);

			}

		processedImage = dilatedImage;
		pictureBox2->Image = surface;



	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the kmeans process first !");
}

//Mean based noise filter
System::Void MyForm::meanButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (intensityControl)
	{
		/*BYTE* filteredImage = new BYTE[imageHeight * imageWidth];
		for (int i = 0; i < imageHeight * imageWidth; i++)
			filteredImage[i] = 0;*/

		BYTE* filteredImage = new BYTE[imageHeight * imageWidth];               //�er�evesi orijinal resim olan i�i bo� dizi
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;

				if (!row || !col || row == imageHeight - 1 || col == imageWidth - 1)
					filteredImage[pos] = processedImage[pos];
				else
					filteredImage[pos] = 0;
			}

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				int values[9] =
				{
					processedImage[pos - 1 - imageWidth],
					processedImage[pos - imageWidth],
					processedImage[pos + 1 - imageWidth],
					processedImage[pos - 1],
					processedImage[pos],
					processedImage[pos + 1],
					processedImage[pos - 1 + imageWidth],
					processedImage[pos + imageWidth],
					processedImage[pos + 1 + imageWidth]
				};

				int mean = 0;
				for (int i = 0; i < 9; i++)
					mean += values[i] / 9;

				filteredImage[pos] = (BYTE)mean;
			}

		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		Color pixelColor;

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				pixelColor = Color::FromArgb(filteredImage[pos], filteredImage[pos], filteredImage[pos]);
				surface->SetPixel(col, row, pixelColor);
			}

		pictureBox2->Image = surface;
		processedImage = filteredImage;
	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the intensity process first !");

}

//Median based noise filter
System::Void MyForm::medianButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (intensityControl)
	{
		/*BYTE* filteredImage = new BYTE[imageHeight * imageWidth];
		for (int i = 0; i < imageHeight * imageWidth; i++)
			filteredImage[i] = 0;*/

		BYTE* filteredImage = new BYTE[imageHeight * imageWidth];            //�er�evesi orijinal resim olan i�i bo� dizi
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;

				if (!row || !col || row == imageHeight - 1 || col == imageWidth - 1)
					filteredImage[pos] = processedImage[pos];
				else
					filteredImage[pos] = 0;
			}

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				int values[9] =
				{
					processedImage[pos - 1 - imageWidth],
					processedImage[pos - imageWidth],
					processedImage[pos + 1 - imageWidth],
					processedImage[pos - 1],
					processedImage[pos],
					processedImage[pos + 1],
					processedImage[pos - 1 + imageWidth],
					processedImage[pos + imageWidth],
					processedImage[pos + 1 + imageWidth]
				};

				int min, temp;								//bu de�erleri s�ralay�p ortadaki de�eri al�yorum
				for (int i = 0; i < 8; i++)
				{
					min = i;
					for (int j = i + 1; j < 9; j++)
						if (values[j] < values[min])
							min = j;
					temp = values[i];
					values[i] = values[min];
					values[min] = temp;
				}

				filteredImage[pos] = values[4];
			}

		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		Color pixelColor;

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				pixelColor = Color::FromArgb(filteredImage[pos], filteredImage[pos], filteredImage[pos]);
				surface->SetPixel(col, row, pixelColor);
			}

		pictureBox2->Image = surface;
		processedImage = filteredImage;

	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the intensity process first !");
}

//Calculate the moments of objects
System::Void MyForm::learningButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (markerControl)
	{
		unsigned char** imagePiece = new unsigned char* [objectCount];    //her bir objeye ait resim par�as�n� tutacak olan dizileri saklayan pointer dizisi

		double** objectMoments = new double* [objectCount];        //objelere ait 7 farkl� moment de�erini saklayacak olan dizi
		for (int i = 0; i < objectCount; i++)
			objectMoments[i] = new double[7];

		int window[4];
		int windowWidth, windowHeight, windowPos;
		std::ifstream reader("objectPositions.txt");

		for (int i = 0; i < objectCount; i++)  //obje say�s� kadar her bir ojje i�in dosyadan okuyarak bu objeleri k���k resim par�alar� halinde imagePiece dizisine aktar�yorum 
		{									   //ve bu par�alara ait 7 farkl� moment de�erini objectMoments dizisine kaydedip bir txt dosyas�na her bir de�erin aritmetik ortalamalar�n� yaz�yorum
			reader >> window[0];
			reader >> window[1];
			reader >> window[2];
			reader >> window[3];

			windowWidth = window[2] - window[0] + 1;
			windowHeight = window[3] - window[1] + 1;
			imagePiece[i] = new unsigned char[windowWidth * windowHeight];

			windowPos = 0;

			for (int row = window[1]; row <= window[3]; row++)
				for (int col = window[0]; col <= window[2]; col++)
				{
					int pos = row * imageWidth + col;
					imagePiece[i][windowPos] = image255[pos];
					windowPos++;
				}

			myHuMoments* temp = new myHuMoments(imagePiece[i], windowWidth, windowHeight);
			//myHuMoments huMomentsObj(imagePiece[i], windowWidth, windowHeight);
			objectMoments[i] = temp->getHuMoments();
			temp->~myHuMoments();
		}
		reader.close();

		std::ofstream writer("objectMoments.txt");

		double temp;
		for (int i = 0; i < 7; i++)
		{
			temp = 0;
			for (int j = 0; j < objectCount; j++)
				temp += objectMoments[j][i] / objectCount;
			writer << temp << " ";
		}
		writer.close();

		/*int asd = 0;
		for (int i = 0; i < imageWidth * imageHeight; i++)
		{
			if (image255[i] != (BYTE)0 && image255[i] != (BYTE)255) asd++;
		}
		System::Windows::Forms::MessageBox::Show(" " + asd);*/
	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the marker process first !");

}

//Finding objects by moment values
System::Void MyForm::findingButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (markerControl)
	{

		double** savedMoments = new double* [7];
		for (int i = 0; i < 7; i++)
			savedMoments[i] = new double[7];

		double YMercimek[7] = { 0.000641218, 1.78701e-08, 7.38951e-13, 5.34067e-14, 4.31811e-26, 9.88018e-18, 4.86388e-27 };//Ye�il
		double KMercimek[7] = { 0.000671783, 6.31436e-08, 1.2755e-12, 1.50889e-13, 5.57229e-25, 6.94845e-17, 4.30592e-25 };//K�rm�z�
		double Pirinc[7] = { 0.000850257, 3.23333e-07, 1.43089e-11, 1.17783e-12, -1.32337e-24, -3.16679e-16, -2.24316e-24 };//Sar�
		double Fasulye[7] = { 0.000734864, 6.05305e-08, 8.87971e-12, 3.91783e-12, -4.03264e-24, 6.31098e-16, -8.22573e-23 };//Mor
		double Cekirdek[7] = { 0.00101535, 6.51629e-07, 4.345e-11, 1.44893e-11, 5.79745e-22, 1.15439e-14, 1.03524e-22 };//Pembe
		double Ceviz[7] = { 0.000805207, 2.16517e-07, 2.64535e-11, 1.74554e-12, 1.07621e-23, 5.31172e-16, -1.56615e-23 };//Mavi
		double Eriste[7] = { 0.00360198, 1.24532e-05, 1.80371e-09, 6.74603e-10, 1.47702e-18, 1.40608e-12, -1.00958e-18 };//Turanj

		savedMoments[0] = YMercimek;
		savedMoments[1] = KMercimek;
		savedMoments[2] = Pirinc;
		savedMoments[3] = Fasulye;
		savedMoments[4] = Cekirdek;
		savedMoments[5] = Ceviz;
		savedMoments[6] = Eriste;

		unsigned char** imagePiece = new unsigned char* [objectCount];

		double** objectMoments = new double* [objectCount];
		for (int i = 0; i < objectCount; i++)
			objectMoments[i] = new double[7];

		int window[4];
		int windowWidth, windowHeight, windowPos;
		std::ifstream reader("objectPositions.txt");

		for (int i = 0; i < objectCount; i++)
		{
			reader >> window[0];
			reader >> window[1];
			reader >> window[2];
			reader >> window[3];

			windowWidth = window[2] - window[0] + 1;
			windowHeight = window[3] - window[1] + 1;
			imagePiece[i] = new unsigned char[windowWidth * windowHeight];

			windowPos = 0;

			for (int row = window[1]; row <= window[3]; row++)
				for (int col = window[0]; col <= window[2]; col++)
				{
					int pos = row * imageWidth + col;
					imagePiece[i][windowPos] = image255[pos];
					windowPos++;
				}

			myHuMoments* temp = new myHuMoments(imagePiece[i], windowWidth, windowHeight);
			//myHuMoments huMomentsObj(imagePiece[i], windowWidth, windowHeight);
			objectMoments[i] = temp->getHuMoments();
			temp->~myHuMoments();
		}
		reader.close();

		double distance;
		int* objectTagArray = new int[objectCount]; // 1,2,3,4,5,6 ve 7 lerden olu�an bir dizi, objenin hangi tip obje oldu�unu g�sterir

		for (int i = 0; i < objectCount; i++)
		{
			distance = Double::MaxValue;
			for (int j = 0; j < 7; j++)
			{
				//Objeyi moment de�erlerine g�re s�n�fland�rma. �klid uzakl���na g�re kime daha yak�nsa ona g�re numaraland�r�p renklendiriyorum
				if (EuclideanDistance(objectMoments[i], savedMoments[j]) < distance)
				{
					distance = EuclideanDistance(objectMoments[i], savedMoments[j]);
					objectTagArray[i] = j + 1;
				}
			}

		}

		/*for (int i = 0; i < 7; i++)
			System::Windows::Forms::MessageBox::Show(" " + objectMoments[0][i]);*/

		for (int i = 0; i < imageWidth * imageHeight; i++)
			if (markedImage[i]) markedImage[i] = objectTagArray[markedImage[i] - 1];

		Bitmap^ surface = gcnew Bitmap(imageWidth, imageHeight);

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				if (markedImage[pos] == 255)
					surface->SetPixel(col, row, Color::Black);
				else if (markedImage[pos] == 1)
					surface->SetPixel(col, row, Color::Green);
				else if (markedImage[pos] == 2)
					surface->SetPixel(col, row, Color::Red);
				else if (markedImage[pos] == 3)
					surface->SetPixel(col, row, Color::Yellow);
				else if (markedImage[pos] == 4)
					surface->SetPixel(col, row, Color::Purple);
				else if (markedImage[pos] == 5)
					surface->SetPixel(col, row, Color::Pink);
				else if (markedImage[pos] == 6)
					surface->SetPixel(col, row, Color::Blue);
				else if (markedImage[pos] == 7)
					surface->SetPixel(col, row, Color::Orange);
				else if (markedImage[pos] == 0)
					surface->SetPixel(col, row, Color::White);
			}
		pictureBox2->Image = surface;

	}
	else
		System::Windows::Forms::MessageBox::Show("You must run the marker process first !");

}

//�klid uzakl��� bulma
double MyForm::EuclideanDistance(double* arr1, double* arr2)
{
	int momentsSize = 7;
	double temp = 0;
	for (int i = 0; i < momentsSize; i++)
	{
		temp += pow((arr1[i] - arr2[i]), 2);
	}
	return sqrt(temp);
}

//Kenar bulma algoritmas�
System::Void MyForm::cannyButton_Click(System::Object^ sender, System::EventArgs^ e)
{

	if (intensityControl && !kmeansControl)
	{

		// burdan itibaren kontrol et toplamlar� ve g�r�nt� hatal�

		BYTE* byteSobel = new BYTE[imageWidth * imageHeight];

		//creating a temp X image, yatay t�rev
		int* sobelImageX = new int[imageHeight * imageWidth];
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				sobelImageX[pos] = (int)0;
			}

		//creating X mask //t�rev ve gaus filtresi matrisi
		int gx[9] =
		{
			1, 0, -1,
			2, 0, -2,
			1, 0, -1
		};



		//filling the temp X image
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				sobelImageX[pos] =
					processedImage[pos - 1 - imageWidth] * gx[0] +
					processedImage[pos - imageWidth] * gx[1] +
					processedImage[pos + 1 - imageWidth] * gx[2] +
					processedImage[pos - 1] * gx[3] +
					processedImage[pos] * gx[4] +
					processedImage[pos + 1] * gx[5] +
					processedImage[pos - 1 + imageWidth] * gx[6] +
					processedImage[pos + imageWidth] * gx[7] +
					processedImage[pos + 1 + imageWidth] * gx[8];

			}

		//filling the byteimage with X image and saving it
		for (int i = 0; i < imageWidth * imageHeight; i++)
			byteSobel[i] = BYTE(sobelImageX[i] / 4);
		Save(byteSobel, imageWidth, imageHeight, imageSize);
		//

		//creating a temp Y image, dikey t�rev
		int* sobelImageY = new int[imageHeight * imageWidth];
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				sobelImageY[pos] = (int)0;
			}

		//creating Y mask //t�rev ve gaus filtresi matrisi
		int gy[9] =
		{
			1, 2, 1,
			0, 0, 0,
		   -1,-2,-1
		};

		//filling the temp Y image
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				sobelImageY[pos] =
					processedImage[pos - 1 - imageWidth] * gy[0] +
					processedImage[pos - imageWidth] * gy[1] +
					processedImage[pos + 1 - imageWidth] * gy[2] +
					processedImage[pos - 1] * gy[3] +
					processedImage[pos] * gy[4] +
					processedImage[pos + 1] * gy[5] +
					processedImage[pos - 1 + imageWidth] * gy[6] +
					processedImage[pos + imageWidth] * gy[7] +
					processedImage[pos + 1 + imageWidth] * gy[8];

			}

		//filling the byteimage with Y image and saving it
		for (int i = 0; i < imageWidth * imageHeight; i++)
			byteSobel[i] = BYTE(sobelImageY[i] / 4);
		Save(byteSobel, imageWidth, imageHeight, imageSize);
		//

		//t�revlerin oran�ndan pozisyon de�erlerine ait gradient a�� dizisi yarat�yoruz //kenara dik olacak a�� dizisi
		double* angleArray = new double[imageHeight * imageWidth];
		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				angleArray[pos] = (double)0;
			}

		for (int row = 0; row < imageHeight; row++)
			for (int col = 0; col < imageWidth; col++)
			{
				int pos = row * imageWidth + col;
				angleArray[pos] = atan2(sobelImageY[pos] * 1., sobelImageX[pos] * 1.) * 180 / PI;
			}

		int* sobelImageTotal = new int[imageHeight * imageWidth];
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				sobelImageTotal[pos] = abs(sobelImageX[pos]) + abs(sobelImageY[pos]);
			}

		//filling the byteimage with Total image and saving it
		for (int i = 0; i < imageWidth * imageHeight; i++)
			byteSobel[i] = sobelImageTotal[i];
		Save(byteSobel, imageWidth, imageHeight, imageSize);
		//

		//Non-maxima supression //noktan�n a��s�na g�re y�nlerine bak�p herhangi birinden k���kse 0 yap�yoruz //pixelin �n� ve arkas�na bak�yoruz //kenar�n �st� ve alt� yani
		for (int i = 0; i < imageHeight * imageWidth; i++)
			sobelImageTotal[i] /= 4;

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				int front, back;

				if ((angleArray[pos] >= -22.5 && angleArray[pos] < 22.5) || (angleArray[pos] >= -180 && angleArray[pos] < -157.5) || (angleArray[pos] >= 157.5 && angleArray[pos] <= 180))
				{
					front = 1; back = -1;
				}
				else if ((angleArray[pos] >= 22.5 && angleArray[pos] < 67.5) || (angleArray[pos] >= -157.5 && angleArray[pos] < -112.5))
				{
					front = -imageWidth + 1; back = imageWidth - 1;
				}
				else if ((angleArray[pos] >= 67.5 && angleArray[pos] < 112.5) || (angleArray[pos] >= -112.5 && angleArray[pos] < -67.5))
				{
					front = -imageWidth; back = imageWidth;
				}
				else if ((angleArray[pos] >= 112.5 && angleArray[pos] < 157.5) || (angleArray[pos] >= -67.5 && angleArray[pos] < -22.5))
				{
					front = -imageWidth - 1; back = imageWidth + 1;
				}

				if ((sobelImageTotal[pos] < sobelImageTotal[pos + front]) || (sobelImageTotal[pos] < sobelImageTotal[pos + back]))
					sobelImageTotal[pos] = 0;

			}

		//filling the byteimage with 1st movement and saving it
		for (int i = 0; i < imageWidth * imageHeight; i++)
			byteSobel[i] = (BYTE)sobelImageTotal[i];
		Save(byteSobel, imageWidth, imageHeight, imageSize);
		//

		//Hysteresis Thresholding   //g�r�nt�deki en b�y�k parlakl�k de�erini base treshold yap�p di�erlerini bu de�erin yuzde 40 ve 60 � olarak setliyoruz
									//ve g�r�nt�y� bu de�erin alt�nda ve �st�nde kalan pixellere g�re 255 ve 0 ile dolduruyoruz //�izgiyi inceltiyoruz
		double baseTreshold = 0, treshold2, treshold3;
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				if (sobelImageTotal[pos] > baseTreshold)
					baseTreshold = sobelImageTotal[pos];
			}

		treshold2 = baseTreshold * 40 / 100;
		treshold3 = baseTreshold * 60 / 100;

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				if (sobelImageTotal[pos] <= treshold2) sobelImageTotal[pos] = 0;
				if (sobelImageTotal[pos] >= treshold3) sobelImageTotal[pos] = 255;
			}

		//Save
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				byteSobel[pos] = sobelImageTotal[pos];
			}
		Save(byteSobel, imageWidth, imageHeight, newSize);
		//

		//Clarify edges between thresholds (Continue Hyteresis Thresholding)
		//g�r�nt�deki kenarlar�n a��lar� (kenara dik olan a��lar� kenar a��s�na �eviriyoruz) //�izgiyi belirginle�tiriyoruz. //kenar�n �zerinde sa� ve sol
		//buras� recursive olmal�
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				int left, right;
				if ((angleArray[pos] >= -22.5 && angleArray[pos] < 22.5) || (angleArray[pos] >= -180 && angleArray[pos] < -157.5) || (angleArray[pos] >= 157.5 && angleArray[pos] <= 180))
				{
					left = -imageWidth; right = imageWidth;
				}
				else if ((angleArray[pos] >= 22.5 && angleArray[pos] < 67.5) || (angleArray[pos] >= -157.5 && angleArray[pos] < -112.5))
				{
					left = -imageWidth - 1; right = imageWidth + 1;
				}
				else if ((angleArray[pos] >= 67.5 && angleArray[pos] < 112.5) || (angleArray[pos] >= -112.5 && angleArray[pos] < -67.5))
				{
					left = -1; right = +1;
				}
				else if ((angleArray[pos] >= 112.5 && angleArray[pos] < 157.5) || (angleArray[pos] >= -67.5 && angleArray[pos] < -22.5))
				{
					left = -imageWidth + 1; right = imageWidth - 1;
				}

				if (sobelImageTotal[pos] != 0 && sobelImageTotal[pos] != 255)
				{
					if (sobelImageTotal[pos + left] == 255 && sobelImageTotal[pos + right] == 255)
						sobelImageTotal[pos] = 255;
					else if (sobelImageTotal[pos + left] == 0 && sobelImageTotal[pos + right] == 0)
						sobelImageTotal[pos] = 0;
				}

			}

		//Save
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;
				byteSobel[pos] = sobelImageTotal[pos];
			}
		Save(byteSobel, imageWidth, imageHeight, imageSize);
		//

		//Convert angleArray from gradient angle to edge angle //kenarlara dik a��lar yerine pixellerin a��lar�n� //yani kenar a��lar�
		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				angleArray[pos] += 90;

				if (angleArray[pos] > 180)
					angleArray[pos] -= 360;
			}

		//Hough Transform
		int dMax = round(sqrt(pow(imageWidth, 2) + pow(imageHeight, 2)));  //g�r�nt�deki ilk pixelin en uzak kenara olan uzakl���
		int* houghSpace = new int[dMax * 360];

		for (int row = 0; row < dMax; row++)
			for (int col = 0; col < 360; col++)
			{
				int pos = row * 360 + col;
				houghSpace[pos] = 0;
			}

		int errorTreshold = 1;
		double d;
		int houghIndex;

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				if (sobelImageTotal[pos] == 255)
				{
					if (angleArray[pos] < 0)
						angleArray[pos] += 360;  //- de�erleri 180 360 aras� de�erlere �eviriyoruz

					if ((angleArray[pos] > (270 - errorTreshold) && angleArray[pos] < (270 + errorTreshold)) || (angleArray[pos] > (90 - errorTreshold) && angleArray[pos] < (90 + errorTreshold)))
						angleArray[pos] = 90; //dikey kenar
					if ((angleArray[pos] > (180 - errorTreshold) && angleArray[pos] < (180 + errorTreshold)) || (angleArray[pos] < (0 + errorTreshold)) || (angleArray[pos] > (360 - errorTreshold)))
						angleArray[pos] = 0;  //yatay kenar


					if (angleArray[pos] == 0 || angleArray[pos] == 90) //�art� sa�l�yorsa bu noktan�n kenar olma olas�l��� var demektir
					{
						//d = round(sqrt(pow(row + 1, 2) + pow(col + 1, 2)));
						d = abs(row * round(sin(angleArray[pos] * PI / 180)) + col * round(cos(angleArray[pos] * PI / 180)));
						houghIndex = d * 360 + angleArray[pos];
						houghSpace[houghIndex]++;
					}
				}
			}

		//saving houghSpace
		BYTE* houghSave = new BYTE[dMax * 360];
		for (int row = 0; row < dMax; row++)
			for (int col = 0; col < 360; col++)
			{
				int pos = row * 360 + col;

				if (houghSpace[pos] > 5)
					houghSave[pos] = 255;
				else
					houghSave[pos] = 0;
			}
		Save(houghSave, dMax, 360, imageSize);

		//Finding the Max 16 values in hough space
		int maxHough[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 };
		int dHough[16];
		int angHough[16];
		int max = 0;
		int dHoughMax;
		int angHoughMax;

		//hough uzay�ndaki max de�erleri bulup ilgili de�i�kenleri setliyoruz
		for (int row = 0; row < dMax; row++)
			for (int col = 0; col < 360; col++)
			{
				int pos = row * 360 + col;
				//int imagePos = (pos - col) / 360;

				if (houghSpace[pos] > max)
				{
					max = houghSpace[pos];
					dHoughMax = row;
					angHoughMax = col;
				}
			}

		maxHough[0] = max;
		dHough[0] = dHoughMax;
		angHough[0] = angHoughMax;


		for (int k = 1; k < 16; k++)
		{
			max = 0;
			for (int row = 0; row < dMax; row++)
				for (int col = 0; col < 360; col++)
				{
					int pos = row * 360 + col;
					//int imagePos = (pos - col) / 360;

					if (houghSpace[pos] > max && houghSpace[pos] < maxHough[k - 1])
					{
						max = houghSpace[pos];
						dHoughMax = row;
						angHoughMax = col;
					}
				}
			maxHough[k] = max;
			dHough[k] = dHoughMax;
			angHough[k] = angHoughMax;
		}//Found the max values


		/*int maxVal = 0;
		for (int row = 0; row < dMax; row++)
			for (int col = 0; col < 360; col++)
			{
				int pos = row * 360 + col;

				if (houghSpace[pos] > maxVal)
					maxVal = houghSpace[pos];
			}
		System::Windows::Forms::MessageBox::Show(" " + maxVal);*/


		System::Windows::Forms::MessageBox::Show(maxHough[0] + " " + maxHough[1] + " " + maxHough[2] + " " + maxHough[3] + " " + maxHough[4] + " " + maxHough[5] + " " + maxHough[6] + " " + maxHough[7] + " " + maxHough[8] + " " + maxHough[9] + " " + maxHough[10] + " " + maxHough[11] + " " + maxHough[12] + " " + maxHough[13] + " " + maxHough[14] + " " + maxHough[15] + " ");

		int r[16], c[16];

		for (int i = 0; i < 16; i++)
		{
			r[i] = -1;
			c[i] = -1;
		}

		for (int k = 0; k < 16; k++)
		{
			if (angHough[k] == 0)
				r[k] = dHough[k];

			else if (angHough[k] == 90)
				c[k] = dHough[k];
		}

		for (int row = 1; row < imageHeight - 1; row++)
			for (int col = 1; col < imageWidth - 1; col++)
			{
				int pos = row * imageWidth + col;

				for (int i = 0; i < 16; i++)
				{
					if (c[i] == row || r[i] == col)   // dikey ise cos 0 gelir row de�erine e�it olmal�, yatay ise de tam tersi
					{
						byteSobel[pos] = (BYTE)255;
						break;
					}
					else byteSobel[pos] = sobelImageTotal[pos];
				}

			}

		Save(byteSobel, imageWidth, imageHeight, imageSize);
		pictureBox2->ImageLocation = ("Images//Outs//Out" + (processCount - 1) + ".bmp");

	}
	else if (!intensityControl)
		System::Windows::Forms::MessageBox::Show("You must run the intensity process first !");
	else
		System::Windows::Forms::MessageBox::Show("ProcessedImage array has been changed because of running kmeans process, please run this process without using kmeans");

}

//save
Void MyForm::Save(BYTE* Buffer, int widthf, int heightf, long ssize)
{
	BYTE* printBuffer = ConvertIntensityToBMP(Buffer, widthf, heightf, ssize);
	CString Location = "Images//Outs//Out" + processCount + ".bmp";
	output = Location;
	if (SaveBMP(printBuffer, widthf, heightf, ssize, output))
		System::Windows::Forms::MessageBox::Show("Image successfully saved.");
	else
		System::Windows::Forms::MessageBox::Show("Error saving image.");
	processCount++;
	delete[] printBuffer;
}

