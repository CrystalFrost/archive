using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace CutImage
{
    public partial class mainForm : Form
    {
        public mainForm()
        {
            InitializeComponent();
        }

        private void CropToFullHD()
        {
            string inputDirectoryPath = @"E:\Science\Astronomy\Universe\Solar System\0 - Sun\Active Region 2529\Line_1700_Story\source\";
            string outputDirectoryPath = @"E:\Science\Astronomy\Universe\Solar System\0 - Sun\Active Region 2529\Line_1700_Story\output\";
            string outputFileName;

            string[] fileNames = Directory.GetFiles(inputDirectoryPath);
            List<string> fileList = new List<string>(fileNames);
            fileList.Sort();

            int startMoveFrameId = 400;
            Size frameSize = new Size(1920, 1080);

            int i = 0;
            for (i = 0; i < fileList.Count; i++)
            {
                int xDeviation = 0;

                if (i >= startMoveFrameId)
                {
                    xDeviation = (4096 - frameSize.Width) / (fileList.Count - startMoveFrameId * 2) * (i - startMoveFrameId);
                    if (xDeviation + frameSize.Width > 4096)
                    {
                        xDeviation = 4096 - frameSize.Width;
                    }
                }
                else
                {
                    xDeviation = 0;
                }

                int startX = xDeviation;
                int startY = 1020;

                Point sourceStartPoint = new Point(startX, startY);
                Point targetStartPoint = new Point(0, 0);
                Rectangle sourceRectangle = new Rectangle(sourceStartPoint, frameSize);
                Rectangle targetRectangle = new Rectangle(targetStartPoint, frameSize);

                Bitmap sourceImage = Image.FromFile(fileList[i]) as Bitmap;
                Bitmap targetImage = new Bitmap(frameSize.Width, frameSize.Height);
                Graphics graph = Graphics.FromImage(targetImage);

                graph.DrawImage(sourceImage, targetRectangle, sourceRectangle, GraphicsUnit.Pixel);

                outputFileName = fileList[i].Replace(inputDirectoryPath, outputDirectoryPath).Replace(".jpg", ".png");
                targetImage.Save(outputFileName, System.Drawing.Imaging.ImageFormat.Png);

                GC.Collect();
            }
        }

        private void CropToHD()
        {
            string inputDirectoryPath = @"C:\Users\Maxim\Downloads\SDO\Line_171\full_stack\";
            string outputDirectoryPath = @"C:\Users\Maxim\Downloads\SDO\Line_171\crop\";
            string outputFileName;

            string[] fileNames = Directory.GetFiles(inputDirectoryPath);
            List<string> fileList = new List<string>(fileNames);
            fileList.Sort();

            int startMoveFrameId = 320;
            Size frameSize = new Size(1280, 720);

            int i = 0;
            for (i = 0; i < fileList.Count; i++)
            {
                int xDeviation = 0;

                if (i >= startMoveFrameId)
                {
                    xDeviation = (4096 - frameSize.Width) / (fileList.Count - startMoveFrameId * 2) * (i - startMoveFrameId);
                    if (xDeviation + frameSize.Width > 4096)
                    {
                        xDeviation = 4096 - frameSize.Width;
                    }
                }
                else
                {
                    xDeviation = 0;
                }

                int startX = xDeviation;
                int startY = 1200;

                Point sourceStartPoint = new Point(startX, startY);
                Point targetStartPoint = new Point(0, 0);
                Rectangle sourceRectangle = new Rectangle(sourceStartPoint, frameSize);
                Rectangle targetRectangle = new Rectangle(targetStartPoint, frameSize);

                Bitmap sourceImage = Image.FromFile(fileList[i]) as Bitmap;
                Bitmap targetImage = new Bitmap(frameSize.Width, frameSize.Height);
                Graphics graph = Graphics.FromImage(targetImage);

                graph.DrawImage(sourceImage, targetRectangle, sourceRectangle, GraphicsUnit.Pixel);

                outputFileName = fileList[i].Replace(inputDirectoryPath, outputDirectoryPath).Replace(".jpg", ".png");
                targetImage.Save(outputFileName, System.Drawing.Imaging.ImageFormat.Png);

                GC.Collect();
            }
        }

        private void CombineCrop()
        {
            string inputDirectoryAPath = @"C:\Users\Maxim\Downloads\SDO\Combined\hmi\";
            string inputDirectoryBPath = @"C:\Users\Maxim\Downloads\SDO\Combined\Line_171\";
            string outputDirectoryPath = @"C:\Users\Maxim\Downloads\SDO\Combined\output\";
            string outputFileName;

            List<string> fileListA = new List<string>(Directory.GetFiles(inputDirectoryAPath));
            List<string> fileListB = new List<string>(Directory.GetFiles(inputDirectoryBPath));
            fileListA.Sort();
            fileListB.Sort();

            int framesCount = Math.Min(fileListA.Count, fileListB.Count);
            int startMoveFrameId = 400;
            Size targetFrameSize = new Size(1920, 1080);
            Size sourceFrameSize = new Size(1920, 540);

            int i = 0;
            for (i = 0; i < framesCount; i++)
            {
                int xDeviation = 0;

                if (i >= startMoveFrameId)
                {
                    xDeviation = (4096 - targetFrameSize.Width) / (framesCount - startMoveFrameId * 2) * (i - startMoveFrameId);
                    if (xDeviation + targetFrameSize.Width > 4096)
                    {
                        xDeviation = 4096 - targetFrameSize.Width;
                    }
                }
                else
                {
                    xDeviation = 0;
                }

                int startX = xDeviation;
                int startYA = 1270;
                int startYB = 1370;

                Point sourceStartPointA = new Point(startX, startYA);
                Point sourceStartPointB = new Point(startX, startYB);
                Point targetStartPointA = new Point(0, 0);
                Point targetStartPointB = new Point(0, 540);
                Rectangle sourceRectangleA = new Rectangle(sourceStartPointA, sourceFrameSize);
                Rectangle sourceRectangleB = new Rectangle(sourceStartPointB, sourceFrameSize);
                Rectangle targetRectangleA = new Rectangle(targetStartPointA, sourceFrameSize);
                Rectangle targetRectangleB = new Rectangle(targetStartPointB, sourceFrameSize);

                Bitmap sourceImageA = Image.FromFile(fileListA[i]) as Bitmap;
                Bitmap sourceImageB = Image.FromFile(fileListB[i]) as Bitmap;

                Bitmap targetImage = new Bitmap(targetFrameSize.Width, targetFrameSize.Height);
                Graphics graph = Graphics.FromImage(targetImage);

                graph.DrawImage(sourceImageA, targetRectangleA, sourceRectangleA, GraphicsUnit.Pixel);
                graph.DrawImage(sourceImageB, targetRectangleB, sourceRectangleB, GraphicsUnit.Pixel);

                outputFileName = fileListA[i].Replace(inputDirectoryAPath, outputDirectoryPath).Replace(".jpg", ".png");
                targetImage.Save(outputFileName, System.Drawing.Imaging.ImageFormat.Png);

                GC.Collect();
            }
        }

        private void mainForm_Load(object sender, EventArgs e)
        {
            /*CombineCrop();*/
            CropToFullHD();
        }
    }
}
