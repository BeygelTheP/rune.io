const path = require('path');

module.exports = {
  devtool: 'cheap-module-source-map',
  // debug: true,
  context: path.resolve(__dirname, 'src'),
  entry: path.join(__dirname, 'src', 'index.js'),
  output: {
    path: path.resolve(__dirname, 'dist'),
    publicPath: '/dist/',
    filename: 'app.min.js',
  },
  resolve: {
    // root: path.resolve(__dirname),
    extensions: ['.js', '.json'],
    modules: ['node_modules'],
  },
  module: {
    loaders: [{
      test: /\.css$/,
      exclude: /node_modules/,
      loaders: ['style', 'css?modules'],
    }, {
      test: /node_modules\/.+\.css$/,
      loaders: ['style', 'css'],
    }, {
      test: /\.js$/,
      exclude: /node_modules/,
      loader: 'babel-loader',
      query: {
        cacheDirectory: true,
        presets: ['es2015', 'react', 'stage-1'],
        plugins: ['transform-runtime'],
        env: {
          development: {
            presets: ['react-hmre'],
          },
          production: {
            presets: ['react-optimize'],
          },
        },
      },
    }, {
      test: /\.json$/,
      loader: 'json',
    }, {
      test: /\.woff\d?(\?.+)?$/,
      loader: 'url?limit=10000&mimetype=application/font-woff',
    }, {
      test: /\.ttf?(\?.+)?$/,
      loader: 'url?limit=10000&mimetype=application/octet-stream',
    }, {
      test: /\.eot?(\?.+)?$/,
      loader: 'url?limit=10000',
    }, {
      test: /\.png?(\?.+)?$/,
      loader: 'url?limit=10000&mimetype=image/svg+xml',
    }, {
      test: /\.png$/,
      loader: 'url?limit=10000&mimetype=image/png',
    }, {
      test: /\.gif/,
      loader: 'url?limit=10000&mimetype=image/gif',
    }],
  },
};
