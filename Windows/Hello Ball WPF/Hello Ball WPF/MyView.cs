using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Hello_Ball_WPF
{
    public class MyView : Canvas
    {
        public MyView()
        {

        }

        protected override void OnRender(DrawingContext dc)
        {
            SolidColorBrush mySolidColorBrush = new SolidColorBrush();
            mySolidColorBrush.Color = Colors.LimeGreen;
            Pen myPen = new Pen(Brushes.Blue, 10);
            Rect myRect = new Rect(0, 0, ActualWidth, ActualHeight);
            dc.DrawRectangle(mySolidColorBrush, myPen, myRect);

            FontFamily ff = new FontFamily(new Uri("pack://application:,,,/"), "./#Balsamiq Sans");
            var formattedText = new FormattedText(
                "AVAVAVA and arabic العربية عربي/عربى",
                System.Globalization.CultureInfo.CurrentUICulture,
                FlowDirection.LeftToRight,
                new Typeface(ff, FontStyles.Normal, FontWeights.Normal, FontStretches.Normal),
                24,
                Brushes.Black);
            Size s = new Size(formattedText.Width, formattedText.Height);

            dc.DrawText(formattedText, new Point(20, 20));
        }
    }
}
