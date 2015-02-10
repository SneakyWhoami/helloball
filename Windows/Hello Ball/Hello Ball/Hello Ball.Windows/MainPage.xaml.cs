using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Shapes;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace Hello_Ball
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            this.canvas.Background = new SolidColorBrush(Colors.Red);
            this.SizeChanged += MainPage_SizeChanged;
            this.Loaded += MainPage_Loaded;
        }

        void MainPage_Loaded(object sender, RoutedEventArgs e)
        {
            this.PopulateCanvas();
        }

        void MainPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            this.balls.Width = this.canvas.ActualWidth;
            this.balls.Height = this.canvas.ActualHeight;
        }

        private void Canvas_PointerPressed(object sender, PointerRoutedEventArgs e)
        {
            this.canvas.Background = new SolidColorBrush(Colors.LightBlue);
        }

        private void Canvas_PointerReleased(object sender, PointerRoutedEventArgs e)
        {
            this.canvas.Background = new SolidColorBrush(Colors.Cornsilk);
        }

        private void PopulateCanvas()
        {
            double width = this.balls.ActualWidth;
            double height = this.balls.ActualHeight;
            int ballCount = 30;
            double radius = 10;
            for (int i = 0; i < ballCount; i++)
            {
                Path path = new Path();
                PathGeometry p = new PathGeometry();
                PathFigure f = new PathFigure();

                ArcSegment arc1 = new ArcSegment();
                arc1.Point = new Point(radius * 2, radius);
                arc1.Size = new Size(radius, radius);
                arc1.RotationAngle = 180;
                arc1.IsLargeArc = false;
                arc1.SweepDirection = SweepDirection.Clockwise;
                f.Segments.Add(arc1);

                ArcSegment arc2 = new ArcSegment();
                arc2.Point = new Point(0, radius);
                arc2.Size = new Size(radius, radius);
                arc2.RotationAngle = 180;
                arc2.IsLargeArc = false;
                arc2.SweepDirection = SweepDirection.Clockwise;
                f.Segments.Add(arc2);

                p.Figures.Add(f);
                path.Data = p;
                
                path.Fill = new SolidColorBrush(Colors.Blue);
                path.Stroke = new SolidColorBrush(Colors.Black);
                path.StrokeThickness = 2;

                path.Width = radius * 2;
                path.Height = radius * 2;
                Canvas.SetLeft(path, width / (ballCount + 1) * (i + 1) - radius);
                Canvas.SetTop(path, height / (ballCount + 1) * (i + 1) - radius);
                
                balls.Children.Add(path);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < balls.Children.Count; i++)
            {
                UIElement c = balls.Children[i];
                Canvas.SetLeft(c, Canvas.GetLeft(c) + 20);
            }
        }
    }
}
