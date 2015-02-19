using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Diagnostics;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ChakraHost.Hosting;
using System.Windows.Resources;

namespace Hello_Ball_WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static MainWindow Instance;

        private JavaScriptRuntime runtime;
        private JavaScriptContext context;

        private static JavaScriptNativeFunction onBallCountChanged = OnBallCountChanged;
        private static JavaScriptNativeFunction onDisplayListChanged = OnDisplayListChanged;
        private static JavaScriptNativeFunction onEventsPerSecond = OnEventsPerSecond;
       
        public MainWindow()
        {
            Instance = this;

            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                runtime = JavaScriptRuntime.Create();
                context = runtime.CreateContext();
                JavaScriptContext.Current = context;

                JavaScriptValue hostObject = JavaScriptValue.CreateObject();
                JavaScriptValue globalObject = JavaScriptValue.GlobalObject;
                JavaScriptPropertyId hostPropertyId = JavaScriptPropertyId.FromString("host");
                globalObject.SetProperty(hostPropertyId, hostObject, true);

                JavaScriptValue delegateObject = JavaScriptValue.CreateObject();
                JavaScriptPropertyId delegatePropertyId = JavaScriptPropertyId.FromString("delegate");
                globalObject.SetProperty(delegatePropertyId, delegateObject, true);

                DefineHostCallback(delegateObject, "ballCountChanged", onBallCountChanged, IntPtr.Zero);
                DefineHostCallback(delegateObject, "displayListChanged", onDisplayListChanged, IntPtr.Zero);
                DefineHostCallback(delegateObject, "eventsPerSecond", onEventsPerSecond, IntPtr.Zero);

                Uri uri = new Uri("/model.js", UriKind.Relative);
                StreamResourceInfo info = Application.GetContentStream(uri);
                System.IO.StreamReader sr = new System.IO.StreamReader(info.Stream);
                string script = sr.ReadToEnd();
                JavaScriptValue result = JavaScriptContext.RunScript(script);

                script = "var controller = initApp(" + balls.ActualWidth + ", " + balls.ActualHeight + ", delegate);";
                result = JavaScriptContext.RunScript(script);
            }
            catch (JavaScriptScriptException ex)
            {
                PrintScriptException(ex.Error);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("chakrahost: failed to run script: {0}", ex.Message);
            }
        }
        
        private static JavaScriptValue OnBallCountChanged(JavaScriptValue callee, bool isConstructCall, JavaScriptValue[] arguments, ushort argumentCount, IntPtr callbackData)
        {
            object ballCount = ConvertJavaScriptValue(arguments[1]);
            int bc = (int)(double)ballCount;
            MainWindow.Instance.PopulateCanvas(bc);
            return JavaScriptValue.Undefined;
        }

        private static JavaScriptValue OnDisplayListChanged(JavaScriptValue callee, bool isConstructCall, JavaScriptValue[] arguments, ushort argumentCount, IntPtr callbackData)
        {
            JavaScriptValue displayList = arguments[1];
            object p = ConvertJavaScriptValue(displayList);
            MainWindow.Instance.UpdateBalls(p);
            return JavaScriptValue.Undefined;
        }

        private static object ConvertJavaScriptValue(JavaScriptValue value)
        {
            JavaScriptValueType t = value.ValueType;
            if (t == JavaScriptValueType.Boolean)
            {
                return value.ToBoolean();
            }
            else if (t == JavaScriptValueType.Number)
            {
                return value.ToDouble();
            }
            else if (t == JavaScriptValueType.String)
            {
                return value.ToString();
            }
            else if (t == JavaScriptValueType.Null)
            {
                return null;
            }
            else if (t == JavaScriptValueType.Undefined)
            {
                return null;
            }
            else if (t == JavaScriptValueType.Object)
            {
                Dictionary<string, object> dictionary = new Dictionary<string, object>();
                JavaScriptValue propNames = value.GetOwnPropertyNames();
                
                int i = 0;
                JavaScriptValue index;
                
                while (propNames.HasIndexedProperty(index = JavaScriptValue.FromInt32(i)))
                {
                    JavaScriptValue propName = propNames.GetIndexedProperty(index);
                    JavaScriptPropertyId propId = JavaScriptPropertyId.FromString(propName.ToString());
                    JavaScriptValue propValue = value.GetProperty(propId);
                    object value2 = ConvertJavaScriptValue(propValue);
                    dictionary.Add(propName.ToString(), value2);
                    //Debug.WriteLine(propName.ToString() + ": " + propValue.ConvertToString().ToString());
                    i += 1;
                }

                return dictionary;
            }
            else if (t == JavaScriptValueType.Array)
            {
                JavaScriptPropertyId lengthPropId = JavaScriptPropertyId.FromString("length");
                JavaScriptValue arrayLength = value.GetProperty(lengthPropId);
                int length = (int)arrayLength.ToDouble();
                object[] array = new object[length];

                int i;
                JavaScriptValue index;

                for (i = 0; i < length; i++)
                {
                    if (value.HasIndexedProperty(index = JavaScriptValue.FromInt32(i)))
                    {
                        JavaScriptValue prop = value.GetIndexedProperty(index);
                        array[i] = ConvertJavaScriptValue(prop);
                    }
                }

                return array;
            }
            return null;
        }

        private static JavaScriptValue OnEventsPerSecond(JavaScriptValue callee, bool isConstructCall, JavaScriptValue[] arguments, ushort argumentCount, IntPtr callbackData)
        {
            object eps = ConvertJavaScriptValue(arguments[1]);
            MainWindow.Instance.UpdateEPS((double)eps);
//            Debug.WriteLine("eps: " + eps.ToString());
            return JavaScriptValue.Undefined;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            using (new JavaScriptContext.Scope(context))
            {
                string script = inputText.Text;
                try
                {
                    JavaScriptValue result = JavaScriptContext.RunScript(script);
                    JavaScriptValue stringResult = result.ConvertToString();
                    outputText.Text += stringResult.ToString() + "\n";
                    inputText.SelectAll();
                }
                catch (JavaScriptScriptException ex)
                {
                    PrintScriptException(ex.Error);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("chakrahost: failed to run script: {0}", ex.Message);
                }
            }
        }

        private static void ThrowException(string errorString)
        {
            // We ignore error since we're already in an error state.
            JavaScriptValue errorValue = JavaScriptValue.FromString(errorString);
            JavaScriptValue errorObject = JavaScriptValue.CreateError(errorValue);
            JavaScriptContext.SetException(errorObject);
        }

        private static void PrintScriptException(JavaScriptValue exception)
        {
            //
            // Get message.
            //

            JavaScriptPropertyId messageName = JavaScriptPropertyId.FromString("message");
            JavaScriptValue messageValue = exception.GetProperty(messageName);
            JavaScriptValueType t = messageValue.ValueType;
            string message = messageValue.ToString();

            Debug.WriteLine("chakrahost: exception: {0}", message);
        }

        private static void DefineHostCallback(JavaScriptValue anObject, string callbackName, JavaScriptNativeFunction callback, IntPtr callbackData)
        {
            JavaScriptPropertyId propertyId = JavaScriptPropertyId.FromString(callbackName);
            JavaScriptValue function = JavaScriptValue.CreateFunction(callback, callbackData);
            anObject.SetProperty(propertyId, function, true);
        }

        private void UpdateBalls(object changes)
        {
            Dictionary<string, object> d = (Dictionary<string, object>)changes;
            foreach(KeyValuePair<string, object> entry in d)
            {
                int index = int.Parse(entry.Key);
                Dictionary<string, object> data = (Dictionary<string, object>)entry.Value;
                double x = (double)data["x"];
                double y = (double)data["y"];
                double radius = (double)data["radius"];
                int color = (int)(double)data["color"];
                Path ball = (Path)balls.Children[index];
                ball.Data = MakeBall(radius, 0);
                ball.Width = radius * 2;
                ball.Height = radius * 2;
                ball.Fill = new SolidColorBrush(int2Color(color));
                Canvas.SetLeft(ball, x - radius);
                Canvas.SetTop(ball, y - radius);
                //Debug.WriteLine("ball index: " + entry.Key);
            }
        }

        private void PopulateCanvas(int ballCount)
        {
            double width = balls.ActualWidth;
            double height = balls.ActualHeight;
            double radius = 5;
            for (int i = 0; i < ballCount; i++)
            {
                Path path = new Path();
                path.Data = MakeBall(radius, 0);
                path.Width = radius * 2;
                path.Height = radius * 2;

                path.Fill = new SolidColorBrush(Colors.Blue);
                path.Stroke = new SolidColorBrush(Colors.Black);
                path.StrokeThickness = 1;

                Canvas.SetLeft(path, width / (ballCount + 1) * (i + 1) - radius);
                Canvas.SetTop(path, height / (ballCount + 1) * (i + 1) - radius);

                balls.Children.Add(path);
            }
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var p = e.GetPosition(balls);
            JavaScriptContext.RunScript("controller.mouseDown(" + p.X + ", " + p.Y + ");");
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            var p = e.GetPosition(balls);
            JavaScriptContext.RunScript("controller.mouseMove(" + p.X + ", " + p.Y + ");");
        }

        private void Window_MouseUp(object sender, MouseButtonEventArgs e)
        {
            var p = e.GetPosition(balls);
            JavaScriptContext.RunScript("controller.mouseUp(" + p.X + ", " + p.Y + ");");
        }

        private void UpdateEPS(double value)
        {
            eps.Text = "EPS: " + (int)value;
        }

        private PathGeometry MakeBall(double radius, int color)
        {
            //Path path = new Path();
            PathGeometry p = new PathGeometry();
            PathFigure f = new PathFigure();

            f.StartPoint = new Point(0, radius);

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
            //path.Data = p;

            //path.Fill = new SolidColorBrush(Colors.Blue);
            //path.Stroke = new SolidColorBrush(Colors.Black);
            //path.StrokeThickness = 2;

            //path.Width = radius * 2;
            //path.Height = radius * 2;

            return p;
        }

        private static Color int2Color (int color)
        {
            byte r = (byte)((color >> 16) & 0xFF);
            byte g = (byte)((color >> 8) & 0xFF);
            byte b = (byte)(color & 0xFF);
            return Color.FromRgb(r, g, b);
        }
    }
}
